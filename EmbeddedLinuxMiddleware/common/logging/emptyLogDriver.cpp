#include <string>
#include <iostream>

#include "../basicTypes.h"
#include "../util/EnumByName.h"
#include "../DateTime/DateTime.h"
#include "../util/BasicUtil.h"

#include "LogEntry.h"
#include "ILogDriver.h"
#include "emptyLogDriver.h"

namespace CecilStLabs
{
   emptyLogDriver::emptyLogDriver()
   {
      // intentionally left blank.
   }

   emptyLogDriver::~emptyLogDriver()
   {
      // intentionally left blank.
   }

   void emptyLogDriver::Log(std::string message,
                            eLogLevel level,
                            std::string type)
   {
      std::cout << "emptyLogDriver::Log() - this output isn't expected." << " " << message << std::endl;
   }

   void emptyLogDriver::log(std::string message,
                            eLogLevel level,
                            std::string type,
                            eLogSource source)
   {
      // intentionally left blank.
   }

   LogEntry emptyLogDriver::Parse(std::string message)
   {
      std::cout << "emptyLogDriver::Parse() - this parser isn't expected." << " " << message << std::endl;

      // create and return an 'empty' LogEntry object.
      LogEntry entry(LoggingOFF, DateTime(), std::string(), std::string());
      return entry;
   }
}
