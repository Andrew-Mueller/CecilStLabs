#ifndef LOGENTRY_H_
#define LOGENTRY_H_

namespace CecilStLabs
{
   /**
    * Selection for the amount of information to produce log messages for.
    * These levels are roughly similar to those specified in RFC 54244
    */
   enum eLogLevel
   {
      LoggingOFF           = 0,
      LoggingError         = 1,
      LoggingWarning       = 2,
      LoggingInformation   = 3,
      LoggingVerbose       = 4,
      LoggingTrace         = 5,
      LoggingDebug         = 6,

      MAX_LOG_LEVELS
   };

   /**
    * Source of the log message.  These are the logical software units where the
    * log data is coming from.
    *
    * NOTE: the reserved log sources should be replaced with software units of the specific system.
    */
   enum eLogSource
   {
      LogSourceIrregardless= 0x00,  //!< LogSourceIrregardless NOTE: it is well understood that this is not a word, we are picking on Adam Maslanka of SMT.
      LogSourceComms       = 0x01,  //!< LogSourceComms Communications State Machine and threads
      LogSourceWatchdog    = 0x02,  //!< LogSourceWatchdog Watchdog Driver (initialized, kicked, turned on/off, etc.)
      LogSourceReserved6   = 0x04,
      LogSourceReserved5   = 0x08,
      LogSourceReserved4   = 0x10,
      LogSourceReserved3   = 0x20,
      LogSourceReserved2   = 0x40,
      LogSourceReserved1   = 0x80,

      NUM_LOG_SOURCES      = 9
   };

   /**
    *  Map each of the logging sources to a readable string.
    */
   static const EnumByName logSourceByName[] =
   {
         EnumByName((uint32_t)LogSourceIrregardless,  "Irregardless"),   // NOTE: this is not a word, see enum above for explanation.
         EnumByName((uint32_t)LogSourceComms,         "Comms"),
         EnumByName((uint32_t)LogSourceWatchdog,      "Watchdog"),
         EnumByName((uint32_t)LogSourceReserved6,     "Reserved6"),
         EnumByName((uint32_t)LogSourceReserved5,     "Reserved5"),
         EnumByName((uint32_t)LogSourceReserved4,     "Reserved4"),
         EnumByName((uint32_t)LogSourceReserved3,     "Reserved3"),
         EnumByName((uint32_t)LogSourceReserved2,     "Reserved2"),
         EnumByName((uint32_t)LogSourceReserved1,     "Reserved1")
   };

   /**
    * LogEvent as specified by the Framework documentation.
    */
   class LogEntry
   {
      public:

         /**
          * Create the LogEntry with the specified verbosity level, timestamp,
          * message, and log type.
          *
          * @param level The verbosity level of the log message.
          * @param timestamp The timestamp of the log message.
          * @param message The text description of the log entry message.
          * @param logType A unique identifier for the log entry type.
          */
         LogEntry(const eLogLevel level,
                  const DateTime timestamp,
                  const std::string message,
                  const std::string logType);

         /**
          * release the resources consumed by the LogEntry.
          */
         virtual ~LogEntry();

         /**
          * inline getters for the member variables.
          */
         inline eLogLevel getLogLevel() { return m_eLogLevel; };
         inline DateTime getTimeStamp() { return m_timestamp; };
         inline std::string getMessage() { return m_message; };
         inline std::string getType() { return m_type; };

         /**
          * Retrieves the string representation of the current log level
          */
         std::string getLogLevelName();

         /**
          * Provides the textual format of this log entry.
          */
         std::string toString();

         /**
          * Assignment operator to allow copying one logentry to another.
          */
         LogEntry& operator= (const LogEntry& copy);

         /**
          * Concrete mapping of the strongly typed enumeration for the
          * logging level to the string that will most likely appear
          * in the configuration.
          */
         static const EnumByName LOGLEVEL_NAMES[MAX_LOG_LEVELS];

      protected:

         /**
          * The current logging level.
          */
         eLogLevel   m_eLogLevel;

         /**
          * The time when the log event was created.
          */
         DateTime    m_timestamp;

         /**
          * The description of the log entry
          */
         std::string m_message;

         /**
          * Pre-defined types for messages sent to the server.
          */
         std::string     m_type;
   };
}
#endif
