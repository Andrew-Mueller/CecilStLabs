#ifndef ILOGDRIVER_H
#define ILOGDRIVER_H

namespace CecilStLabs
{
   class LogEntry;

   /**
    * Interface for the logging driver for the specific system.  This defines the
    * contract that the System's Logging driver must meet.  The interface allows
    * the portable code to be able to interface with the logging system while the
    * actual log driver can change for the specific hardware or platform.
    */
   class ILogDriver
   {
      public:

         /**
          * Log the specified message with the specified logging level to the
          * devices error log.
          *
          * @param message Log message to send to the application log.
          * @param level Standard Posix Logging level used to filter messages
          * @param type Option implementation and application specific message
          *             type (defaults to an empty string).
          */
         virtual void Log(std::string message,
                          eLogLevel level,
                          std::string type = "") = 0;

         /**
          * Log the specified message to the device's error log with the
          * specified logging level from the specified software unit as the
          * source.
          *
          * @param message Log message to send to the application log.
          * @param level Standard Posix Logging level used to filter messages.
          * @param type Optional implementation and application specific message
          *             type (defaults to an empty string).
          * @param source Optional implementation and application specific source
          *               of the message.  (defaults to zero).
          */
         virtual void log(std::string message,
                          eLogLevel level,
                          std::string type = "",
                          eLogSource source = LogSourceIrregardless) = 0;

         /**
          * Parse the specified message into a LogEntry Event.
          *
          * @param message String containing a potentially interesting LogEntry
          *
          * @return The LogEntry created from the information read from
          *         the specified message.
          */
         virtual LogEntry Parse(std::string message) = 0;

         /**
          * Sets the Logging Level for the system log.  Any message that is logged
          * with a level that is greater than or equal to the system's logging
          * level will be sent to the system log.  Messages logged with a level
          * that is less than the system's logging level will not be logged.
          *
          * @param newLogLevel The level of logging to filter messages being
          *                    written to the system's log.
          *
          * @return the previous logging level set for the system.
          */
         eLogLevel SetLogLevel(eLogLevel newLogLevel)
         {
            eLogLevel oldLevel = m_logLevel;

            m_logLevel = newLogLevel;

            return oldLevel;
         };

         /**
          * Populate the debug print flag with the one specified.
          *
          * @param debugPrintFlag The bit field to specify the software unit sources
          */
         inline void setDebugPrintFlag(uint8_t debugPrintFlag) { m_debugPrintFlag = debugPrintFlag; };

      protected:

         // NOTE: protect the constructor, copy constructor, assignment operator,
         //       and destructor to ensure the interface can only be created by
         //       the Realization of this Interface.
         ILogDriver() : m_logLevel(LoggingDebug), m_debugPrintFlag(0) { };
         virtual ~ILogDriver() {};

         ILogDriver(ILogDriver* copy __attribute__((unused))) : m_logLevel(LoggingDebug), m_debugPrintFlag(0) { };
         ILogDriver& operator=(ILogDriver& rhs __attribute((unused))) { return rhs; };

         /**
          * The Systems current level of message logging.
          */
         eLogLevel m_logLevel;

         /**
          * The bit field represents the possible sources for debug printing,
          * This field is populated from the system configuration file.
          */
         uint8_t m_debugPrintFlag;
   };
}
#endif
