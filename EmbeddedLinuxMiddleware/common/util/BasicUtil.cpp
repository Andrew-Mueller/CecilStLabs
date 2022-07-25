#include <stdio.h>	// TODO: convert these all to <cstdio>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <errno.h>
#include <signal.h>

#include <iostream>
#include <sstream>
#include <algorithm>
#include <functional>
#include <cctype>
#include <locale>
using namespace std;

#include "../basicTypes.h"
#include "../util/EnumByName.h"
#include "../DateTime/DateTime.h"

#include "../logging/LogEntry.h"
#include "../logging/ILogDriver.h"

#include "BasicUtil.h"

namespace CecilStLabs
{
   // initialize the static data instance.
   BasicUtil* BasicUtil::m_instance = NULL;

   BasicUtil::BasicUtil()
      :
        m_executablePath(""),
        m_logDriver(NULL)
   {
      // intentionally left blank
   }

   BasicUtil::~BasicUtil()
   {
      // intentionally left blank.
   }

   void BasicUtil::Assert(bool conditional,
                          const char* filename,
                          uint16_t lineNumber)
   {
      if (false == conditional)
      {
         // At a minimum print some information out to standard output and standard error.

         ostringstream buff;
         buff << "ASSERT - EPIC FAILURE " << filename << " " << lineNumber << endl;
         buff << "Last error via errno was: " << errno << " " << strerror(errno) << endl;

         if ((NULL != m_instance) &&
             (NULL != m_instance->m_logDriver))
         {
            m_instance->m_logDriver->Log(buff.str(), LoggingError);
         }
         else
         {
            cerr << buff.str() << endl;
         }

         // NOTE: Sending to LogEntry requires communications or at
         //       least still comm buffering is still working, if FileMonitoring is
         //       still up and running, it has a chance of picking it up and sending
         //       it to comms.  Otherwise it will be picked up from the log
         //       the next time the application starts.
         // TODO: this feels very appliciation specific comment ...

         // Intentional infinite loop to trap the assertion.
         uint16_t overflow = 0;

         while(true)    //lint !e716
         {
            // do something so these instructions don't get optimized out...
            overflow++;

            // TODO: other running threads still are getting scheduled.
         }
      }
   }

   void BasicUtil::AssertBigEndian(const char* filename,
                                   uint16_t lineNumber)
   {
      // The principle is equivalent to the "type cast" that is often used to
      // check the byte order but this is clearer - and according to C99, is
      // guaranteed to be correct. gcc prefers this compared to the direct
      // pointer cast that often results in compiler warning and sometimes errors.
      //
      // Cast Example:
      // unsigned char data[] = {0x01, 0x02, 0x03, 0x04};
      // uint32_t cData = (uint32_t)data;
      // return (1 == data[0]);

      // This is also much better than fixing the endianess at compile time - for
      // OS which support multi-architecture (fat binary on Mac os x for example),
      // this will work for both ppc/i386, whereas it is very easy to mess things
      // up otherwise.
      union
      {
         uint32_t i;
         unsigned char fourBytes[4];
      } bint = {0x01020304};

      Assert((1 == bint.fourBytes[0]), filename, lineNumber);
   }

   void BasicUtil::AssertLittleEndian(const char* filename,
                                      uint16_t lineNumber)
   {
      union
      {
         uint32_t i;
         unsigned char fourBytes[4];
      } bint = {0x01020304};

      Assert((4 == bint.fourBytes[0]), filename, lineNumber);
   }

   uint16_t BasicUtil::swapBytes(const uint16_t inputWord)
   {
      uint16_t finalWord = 0;

      // NOTE: the order that the bytes are swapped is not important
      //       high byte first, then low byte or vice-versa.

      // first place the upper byte in the lower bytes position.
      finalWord = (inputWord & 0xFF00) >> 8;

      // next place the lower byte in the upper bytes position.
      finalWord |= (inputWord & 0xFF) << 8;

      return finalWord;
   }

   std::string BasicUtil::FullyQualifiedPath()
   {
      char path[MAX_PATH_LENGTH];
      char* configPath = 0;

      // clear the path to start.
      memset(path, 0, MAX_PATH_LENGTH);

      if ((NULL != m_instance) &&
          (!m_instance->m_executablePath.empty()))
      {
         configPath = (char*)m_instance->m_executablePath.c_str();
      }
      else
      {
		 // TODO: this requires procfs is being used.
         size_t pathLen = readlink("/proc/self/exe", path, MAX_PATH_LENGTH);

         if (-1 == (int)pathLen)
         {
            BasicUtil::Assert(false, __FILE__, __LINE__);
         }
         else
         {
            // the full path will include the executables name, so remove the executable name
            configPath = strrchr(path, '/');
            *(configPath + 1) = '\0';

            // empty the extra characters to be safe.
            memset((configPath + 1), 0, pathLen - strlen(path));

            // return the pointer back to the beginning of the path.
            configPath = (char*)path;
         }

         // if we have already created the static instance by way of setting
         // a logger, then store the value for less work later.
         if ((NULL != m_instance) &&
             (m_instance->m_executablePath.empty()))
         {
            // finally convert the char* magic above to the static std::string
            m_instance->m_executablePath = string(configPath);
         }
      }

      return string(configPath);
   }

   bool BasicUtil::FileExists(std::string filePath)
   {
      bool fileExists = false;

      if (-1 != access(filePath.c_str(), F_OK ))
      {
         fileExists = true;
      }

      return fileExists;
   }

   void BasicUtil::SetLogDriver(ILogDriver* logDriver)
   {
      if (NULL == m_instance)
      {
         m_instance = new BasicUtil();
      }

      m_instance->m_logDriver = logDriver;
   }

   void BasicUtil::log(string message)
   {
      if (NULL != m_instance)
      {
         if (NULL != m_instance->m_logDriver)
         {
            m_instance->m_logDriver->Log(message, LoggingDebug, "");
         }
      }
   }

   uint64_t BasicUtil::ticksPerSecond()
   {
      int64_t ticks = sysconf(_SC_CLK_TCK);

      // NOTE: -1 is returned both if the implementation does not impose a
      //       limit, and in case of an error (via errno).
      BasicUtil::Assert((-1 != ticks), __FILE__, __LINE__);

      // convert the number of ticks to an unsigned integer since the above
      // assert takes care of any negative values, we know the number of ticks
      // is either positive, or the above assert occurred
      return (uint64_t)ticks;
   }

   std::string& BasicUtil::ltrim(std::string& s)
   {
      s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
      return s;
   }


   std::string& BasicUtil::rtrim(std::string& s)
   {
      s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
      return s;
   }


   std::string& BasicUtil::trim(std::string& s)
   {
      return ltrim(rtrim(s));
   }

   void setSignalHandler()
   {
      signal(SIGABRT, genericSignalHandler);
      signal(SIGFPE,  genericSignalHandler);
      signal(SIGILL,  genericSignalHandler);

	  // NOTE: We are currently not catching the interrupt signal so we can use
      //       ctrl+c to stop the application.
      //signal(SIGINT,  genericSignalHandler);

      signal(SIGSEGV, genericSignalHandler);
      signal(SIGTERM, genericSignalHandler);
      signal(SIGPIPE, genericSignalHandler);
   }

   void genericSignalHandler(int sig)
   {
      switch (sig)
      {
         case SIGABRT:
         {
            BasicUtil::log("Caught SIGABRT: usually caused by an abort() or assert()\n");
            break;
         }
         case SIGFPE:
         {
            BasicUtil::log("Caught SIGFPE: arithmetic exception, such as divide by zero or signed integer overflow.\n");
            break;
         }
         case SIGILL:
         {
            BasicUtil::log("Caught SIGILL: illegal instruction\n");
            break;
         }
         case SIGINT:
         {
            BasicUtil::log("Caught SIGINT: interactive attention signal, probably a ctrl+c\n");
            break;
         }
         case SIGSEGV:
         {
            BasicUtil::log("Caught SIGSEGV: segfault\n");
            while(1)
            {
            }
            break;
         }
         case SIGTERM:
         {
            BasicUtil::log("Caught SIGTERM: a termination request was sent to the program\n");
            break;
         }
         case SIGPIPE:
         {
            BasicUtil::log("Caught SIGPIPE: pipe broken on peer side\n");
            break;
         }
         default:
         {
            std::ostringstream omesg;
            omesg << "Unknown Signal handled: ";
            omesg << sig << std::endl;

            BasicUtil::log(omesg.str());
            BasicUtil::Assert(false, __FILE__, __LINE__);
         }
      }

      // should never reach this assert, however, we want to log that we got
      // here... we would never expect to see the following error message.
      BasicUtil::log("Error: program execution should have been trapped before this point.\n");
      BasicUtil::Assert(false, __FILE__, __LINE__);
   }
}
