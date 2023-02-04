#include <string>
#include "../basicTypes.h"
#include "../util/BasicUtil.h"
#include "../util/EnumByName.h"
#include "../DateTime/DateTime.h"

#include "LogEntry.h"

namespace CecilStLabs
{
   /**
    * Fill in the String representation of each of the logging levels.
    * These are exclusively used for reading from the configuration file and
    * printing to the event log.
    */
   const EnumByName LogEntry::LOGLEVEL_NAMES[MAX_LOG_LEVELS] =
   {
       EnumByName(LoggingOFF,          "OFF"),
       EnumByName(LoggingError,        "Error"),
       EnumByName(LoggingWarning,      "Warning"),
       EnumByName(LoggingInformation,  "Information"),
       EnumByName(LoggingVerbose,      "Verbose"),
       EnumByName(LoggingTrace,        "Trace"),
       EnumByName(LoggingDebug,        "Debug")
   };

   LogEntry::LogEntry(const eLogLevel level,
                      const DateTime timestamp,
                      const std::string message,
                      const std::string logType)
      : m_eLogLevel(level),
        m_timestamp(timestamp),
        m_message(message),
        m_type(logType)
   {
      // intentionally left blank.
   }

   LogEntry::~LogEntry()
   {
      // intentionally left blank.
   }

   std::string LogEntry::getLogLevelName()
   {
      // NOTE: It is assumed that this LogEntry must be STRICTLY an eLogLevel
      //       member.
      BasicUtil::Assert((m_eLogLevel >= LoggingOFF && m_eLogLevel < MAX_LOG_LEVELS), __FILE__, __LINE__);

      std::string levelName = LOGLEVEL_NAMES[0].getName();

      for (uint8_t i = 0; i < MAX_LOG_LEVELS; i++)
      {
         if (LOGLEVEL_NAMES[i].getType() == (uint32_t)m_eLogLevel)
         {
            levelName = LOGLEVEL_NAMES[i].getName();
         }
      }

      return levelName;
   }

   LogEntry& LogEntry::operator= (const LogEntry& copy)
   {
      m_eLogLevel = copy.m_eLogLevel;
      m_timestamp = copy.m_timestamp;
      m_message = copy.m_message;
      m_type = copy.m_type;

      return *this;
   }
}
