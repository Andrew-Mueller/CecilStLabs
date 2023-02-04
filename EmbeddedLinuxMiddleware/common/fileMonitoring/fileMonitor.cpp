#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>
#include <sys/select.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string>
#include <iostream>
#include <iomanip>
#include <sstream>
using namespace std;

#include "../basicTypes.h"               //lint !e761
#include "../util/EnumByName.h"
#include "../DateTime/DateTime.h"

#include "../logging/LogEntry.h"
#include "../logging/ILogDriver.h"

#include "../util/BasicUtil.h"
#include "../guid.h"

#include "../util/IFileMonitor.h"
#include "../util/IFileMonitorHandler.h"
#include "fileMonitor.h"

namespace CecilStLabs
{

   fileMonitor::fileMonitor(std::string filePath,
                            IFileMonitorHandler* handler)
      : m_filepath(filePath),
        m_lastModifiedTime(0),
        m_fileHandle(0),
        m_monitorHandler(handler)
   {
      BasicUtil::Assert((true == BasicUtil::FileExists(filePath)), __FILE__, __LINE__);

      m_fileHandle = open(filePath.c_str(), O_RDONLY | O_NONBLOCK);

      BasicUtil::Assert((m_fileHandle > 0), __FILE__, __LINE__);
   }

   fileMonitor::~fileMonitor()
   {
      if (m_fileHandle > 0)
      {
         close((int)m_fileHandle);
      }
   }

   bool fileMonitor::isModified()
   {
      // TODO: below, I am "stat-ing" a named pipe.  I need to understand if
      //       this works the same way as stating a regular file.

      bool isModified = false;

      struct stat file_stat;

      int err = stat(m_filepath.c_str(), &file_stat);

      if (0 != err)
      {
         isModified = false;
      }
      else
      {
         if (file_stat.st_mtime > m_lastModifiedTime)
         {
            m_lastModifiedTime = file_stat.st_mtime;
            isModified = true;
         }
         else if (file_stat.st_ctime > m_lastModifiedTime)
         {
            m_lastModifiedTime = file_stat.st_ctime;
            isModified = true;
         }
      }

      return isModified;
   }

   string fileMonitor::getLinesSinceLastChange()
   {
      std::stringstream nextLine;
      char buffer[MAX_LINE_SIZE];
      bool fullMessage = false;
      int readStatus = 0;

      if (isModified())
      {
         while (!fullMessage)
         {
            // setup the file descriptors for reading.  This is used for select
            // to ensure we don't block on the read.
            fd_set readFiledesc;
            FD_ZERO(&readFiledesc);
            FD_SET(m_fileHandle, &readFiledesc);

            struct timeval timeout;
            timeout.tv_sec = 0;
            timeout.tv_usec = 10;
            int selectStatus = select(m_fileHandle, &readFiledesc, NULL, NULL, &timeout);

            if (0 == selectStatus)
            {
               // clear the buffer
               memset(buffer, 0, MAX_LINE_SIZE);

               // read all of the data from the pipe
               // read 1 less than the max, so that if it fills, it will end in NULL
               readStatus = read(m_fileHandle, buffer, MAX_LINE_SIZE - 1);

               // NOTE: the read call returns the following values:
               // Returns zero to indicate end of file.
               //
               // If no process has the pipe open for writing it will also return 0.
               //
               // If a process is open for writing and O_NONBLOCK is clear, read()
               //    blocks the calling thread until data is written or the pipe is
               //    closed by all processes that had the pipe open for writing
               //
               // If a process is open for writing and O_NONBLOCK is set, -1 is
               //    returned and errno is set to EAGAIN

               if (0 == readStatus)
               {
                  // this shouldn't happen but don't let a zero readStatus get passed into
                  // logic below. It is used as an index and will index BEFORE buffer
                  fullMessage = true;
               }
               else if (0 < readStatus)
               {
                  nextLine << buffer;

                  if ('\n' == buffer[readStatus - 1])
                  {
                     // message contained a new line in it... read the next line too...
                     // but in the final message just append it with a space instead
                     // of the newline.

                     fullMessage = true;
                  }
                  else if ('\0' == buffer[readStatus - 1])
                  {
                     // end of the message?

                     fullMessage = true;
                  }
                  else
                  {
                     // NOTE: we almost ALWAYS expect the message to be less than max...
                  }
               }
               else
               {
                   // read returned negative?
               }
            }
            else
            {
               // TOOD: debug log these error messages somewhere?
               switch(errno)
               {
                  case EBADF :
                  {
                     cout << "EBADF error occurred" << endl;
                     break;
                  }
                  case EINTR :
                  {
                     cout << "EINTR error occurred" << endl;
                     break;
                  }
                  case EINVAL :
                  {
                     cout << "EINVAL error occurred" << endl;
                     break;
                  }
                  case ENOMEM :
                  {
                     cout << "ENOMEM error occurred" << endl;
                     break;
                  }
                  default :
                  {
                     cout << "unknown error occurred" << endl;
                     break;
                  }
               }
               // get out of the loop
               fullMessage = true;
            }
         }

      }

      return nextLine.str();
   }

   void fileMonitor::initialize()
   {
      // intentionally left blank.
   }

   void fileMonitor::monitor()
   {
      // get any lines since last checking
      std::istringstream newMessages(getLinesSinceLastChange());
      while (!newMessages.eof())
      {
         string singleMessage;

         // in the case of multiple lines read, split the message on new lines,
         // each line is a separate message.
         std::getline(newMessages, singleMessage);

         // check size and don't send a zero length string.
         // the getline function doesn't seem to throw out last newline
         if (NULL != m_monitorHandler)
         {
            m_monitorHandler->handle((uint8_t*)singleMessage.c_str(), (uint8_t)singleMessage.length());
         }
      }
   }

   void fileMonitor::destroy()
   {
      // intentionally left blank.
   }
}
