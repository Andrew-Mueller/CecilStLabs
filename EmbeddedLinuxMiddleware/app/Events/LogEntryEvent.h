#ifndef LOGENTRYEVENT_H_
#define LOGENTRYEVENT_H_

namespace CecilStLabs
{
   /**
    * Event data indicates when a device has added a record or multiple records
    * to the system's syslog
    *
    * TODO: currently only contains a single LogEntry per LogEntryEvent, but the
    *       documentation implies that a single LogEntryEvent
    *       can contain MANY LogEntries.
    *       This needs to be refactored to take a "list of LogEntries" in the future
    *       and add them to the LogEntryEvent XML appropriately.
    */
   class LogEntryEvent : public Event
   {
      public:

         /**
          * Create the LogEntryEvent to post containing the necessary info
          * as well as the specified LogEntry data.
          */
         LogEntryEvent(std::string deviceAccessKey,
                       std::string deviceSerialNumber,
                       IClockDriver* clockDriver,
                       CommQueue* commQueue,
                       LogEntry& logEntry);

         /**
          * Release all resources required by the LogEntryEvent.
          */
         virtual ~LogEntryEvent();

         /**
          * Setter for the internal log entry object.
          *
          * @param logEntry The new LogEntry object to use for posting the
          * LogEntryEvent to the server.
          */
         void SetLogEntry(LogEntry& logEntry);

      protected:

         /**
          * The log entry event that we are intending on posting to the server.
          */
         LogEntry& m_logEntry;

         /**
          * Implementation of the Event::BuildXML.  See Event for documentation.
          */
         std::string buildXML();
   };
}
#endif
