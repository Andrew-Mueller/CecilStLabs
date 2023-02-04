#ifndef SYSLOGMONITOR_H_
#define SYSLOGMONITOR_H_

namespace CecilStLabs
{
   /**
    * Monitors the System Log (SysLog) on common linux systems for new messages
    * written.
    */
   class SysLogMonitorThread : public PeriodicThread, public IFileMonitorHandler
   {
      public:
         /**
          * Create the mechanism used to monitor the system log and post messages
          * to the server.
          *
          * @param logEntryEvent The Jetstream LogEntry event used to post the
          *                      messages to the server.
          * @param filePath The path to the system log to monitor for changes.
          */
         SysLogMonitorThread(LogEntryEvent& logEntryEvent,
                             IFileMonitor* fileMonitor,
                             ILogDriver* logDriver);

         /**
          * Release all resources used by the SysLog monitor.
          */
         virtual ~SysLogMonitorThread();

         /**
          * Override the parent PeriodicThread's start condition.
          * Setup the monitoring of the system log.
          */
         void Start();

         /**
          * Override the parent, PeriodicThread's stop condition.
          * Stop the monitoring of the system log.
          */
         void Stop();

         /**
          * The PeriodicThread parent class calls this method at the periodic
          * interval.
          */
         void PeriodicAsyncWorker();

         /**
          * Implementation of the IFileMonitorHandler interface.
          * takes the bytes read from the Event Log and creates a LogEntryEvent
          * to post to the server.
          */
         void handle(uint8_t* bytesRead, uint16_t numBytesRead);

      protected:

         /**
          * The number of seconds for the PeriodicThread to wait between
          * checking for messages.
          */
         static const uint8_t PERIODIC_TIMEOUT_SECONDS = 1;

         /**
          * The object for submitting the properly formatted HTTP POST containing
          * the syslog message.
          */
         LogEntryEvent& m_logEntryEvent;

         /**
          * The abstraction of the file monitoring system used to detect changes
          * in the SysLog.
          */
         IFileMonitor* m_syslogMonitor;

         /**
          * Logging interface for parsing messages from the syslog.
          */
         ILogDriver* m_logDriver;
   };

}
#endif
