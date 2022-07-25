#ifndef LINUXRS232_H_
#define LINUXRS232_H_

namespace CecilStLabs
{
   /**
    * Forward declaration of the baud speed defined in termios.h
    */
   typedef uint32_t speed_t;

   /**
    * Interface to the RS232 device driver using the linux standards.
    */
   class LinuxRS232 : public ISerialDriver
   {
      public:

         /**
          * device driver file path to interact with RS232 with.
          */
         LinuxRS232(std::string deviceFilename, uint32_t baud);

         /**
          * release all resources consumed by the device.
          */
         virtual ~LinuxRS232();

         /**
          * send the specified data out the RS232 point.
          * @param txBuffer is the pointer to the bytes to send out the serial.
          * @param numberOfBytes The number of data bytes to send.
          * @return The actual number of bytes sent.
          */
         uint16_t Send(uint8_t* txBuffer, uint16_t numberOfBytes);

         /**
          * Receive some data into the specified
          *
          * @param rxBuffer is the pointer to the received information.
          * @param numberOfBytes the number of data bytes expected to be received.
          * @return The actual number of bytes read.
          */
         uint16_t Receive(uint8_t* rxBuffer, uint16_t numberOfBytes);

      protected:

         /**
          * filename of the RS232 device to open.
          */
         std::string m_deviceFilename;

         /**
          * The file handle to the RS232 device.
          */
         int16_t m_deviceHandle;

         /**
          * bits per second communication speed.
          */
         speed_t m_baud;
   };

}
#endif
