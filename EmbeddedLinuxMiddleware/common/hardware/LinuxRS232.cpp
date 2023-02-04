#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>

#include <string>
using namespace std;

#include "../basicTypes.h"
#include "../util/BasicUtil.h"

#include "../hardware/ISerialDriver.h"

#include "LinuxRS232.h"

namespace CecilStLabs
{

   LinuxRS232::LinuxRS232(std::string deviceFilename, uint32_t baud)
      : m_deviceFilename(deviceFilename),
        m_baud(baud)
   {
      struct termios term;

      // NOTE: originally, the device handle open contained O_NONBLOCK that caused
      // the reads to be non-blocking, but also caused no reads to succeed successfully.
      //
      // TODO: need to use select and poll along with read to deal with this.
      m_deviceHandle = open(m_deviceFilename.c_str(), O_RDWR | O_NOCTTY);

      BasicUtil::Assert(0 < m_deviceHandle, __FILE__, __LINE__);

      BasicUtil::Assert(isatty(m_deviceHandle), __FILE__, __LINE__);

      BasicUtil::Assert((0 == tcgetattr(m_deviceHandle, &term)), __FILE__, __LINE__);

      // setup the serial port for input modes
      term.c_iflag &= ~(IGNBRK | BRKINT | ICRNL | INLCR | PARMRK | INPCK | ISTRIP | IXON);
      term.c_iflag |= IGNPAR;

      // setup the serial port for output modes
      term.c_oflag = 0;

      // set the serial port's control modes
      term.c_cflag &= ~(CSIZE | PARENB);
      term.c_cflag |= CS8;

      // set the serial port's local modes
      term.c_lflag &= ~(ECHO | ECHONL | ICANON | IEXTEN | ISIG);

      term.c_cc[VMIN] = 1;
      term.c_cc[VTIME] = 0;

      cfmakeraw(&term);

      // convert the baud to the termios speed_t
      // NOTE: we only support specific speeds right now, this is not general purpose.
      if (baud == 19200)
      {
         m_baud = B19200;
      }
      else if (baud == 115200)
      {
         m_baud = B115200;
      }
      else
      {
         // TODO: this is currently only used for 19200.
         //       add additional baud speeds if needed.

         BasicUtil::Assert(false, __FILE__, __LINE__);
      }

      // set both the output speed and input speed as specified.
      cfsetospeed(&term, m_baud);
      cfsetispeed(&term, m_baud);

      // set new device settings IMMEDIATELY (now)
      BasicUtil::Assert((0 == tcsetattr(m_deviceHandle, TCSAFLUSH, &term)), __FILE__, __LINE__);

   }

   LinuxRS232::~LinuxRS232()
   {
      if (m_deviceHandle > 0)
      {
         close(m_deviceHandle);
         m_deviceHandle = -1;
      }
   }

   uint16_t LinuxRS232::Send(uint8_t* data, uint16_t length)
   {
      size_t written = 0;

      BasicUtil::Assert((0 != m_deviceHandle), __FILE__, __LINE__);

      written = write(m_deviceHandle, data, length);

      BasicUtil::Assert((written == (size_t)length), __FILE__, __LINE__);

      return written;
   }

   uint16_t LinuxRS232::Receive(uint8_t* data, uint16_t length)
   {
      size_t bytes_read = 0;

      BasicUtil::Assert((0 != m_deviceHandle), __FILE__, __LINE__);

      bytes_read = read(m_deviceHandle, data, length);

      return bytes_read;
   }
}
