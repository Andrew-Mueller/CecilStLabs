#ifndef SYNCTIMEREQUEST_H_
#define SYNCTIMEREQUEST_H_

namespace CecilStLabs
{
   /**
    * Synchronizes the system time with the server time.
    */
   class SyncTime_Request : public ScheduledTask, public ILoggable
   {
      public:

         /**
          * Create the specified Sync Time request with the specified clock
          * driver and the access key.
          */
         SyncTime_Request(IClockDriver* clockDriver,
                          CommQueue& commQueue,
                          std::string deviceAccessKey,
                          uint8_t hour);

         virtual ~SyncTime_Request();

         /**
          * Request the current UTC time from Jetstream
          */
         bool Request();

         /**
          * The action that implements the ScheduledTask::Task.
          */
         void Task();

      protected:

         /**
          * Interface to the hardware clock driver.
          */
         IClockDriver* m_clockDriver;

         /**
          * Communication queue used for sending message to the Server
          * and persisting data if communication network is down.
          */
         CommQueue& m_commQueue;

         /**
          * Specific unsecured Device URL to get the server time from.
          * This is necessary, in addition to, the secured URL in the case that
          * our local system time is so far off that SSL will not allow us to
          * communicate.
          */
         const static std::string m_unsecuredSyncTimeURL;

         /**
          * Specific Device URL to get the device time from.
          */
         std::string m_securedSyncTimeURL;

         /**
          * The access key for the device
          */
         std::string m_deviceAccessKey;
   };
}
#endif
