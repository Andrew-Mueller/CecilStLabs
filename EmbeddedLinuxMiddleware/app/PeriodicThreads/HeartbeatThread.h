#ifndef HEARTBEATTHREAD_H_
#define HEARTBEATTHREAD_H_

namespace CecilStLabs
{
   /**
    * PeriodicThread that posts the heartbeat message to the server at the
    * frequency specified in number of seconds.
    */
   class HeartbeatThread : public PeriodicThread, public ILoggable
   {
      public:

         /**
          * Create the Heartbeat Thread with the specified device access data
          * and number of seconds to define the heartbeat's period.
          *
          * @param deviceAccessKey The guid that provides access ot post
          *        messages for the device.
          *
          * @param deviceSerialNumber The human readable serial number for the
          *        device for example: NMB000100001
          *
          * @param commQueue The communication queue for sending messages to the
          *        server.
          *
          * @param clockDriver The clock for grabbing the time of the heartbeat
          *
          * @param heartbeatPeriod_minutes The number of minutes to wait
          *        between posting heartbeats (dropping the beat!).
          */
         HeartbeatThread(std::string deviceAccessKey,
                         std::string deviceSerialNumber,
                         CommQueue* commQueue,
                         IClockDriver* clockDriver,
                         uint16_t hearbeatPeriod_minutes);

         /**
          * Release any resources used by the heartbeat thread.
          */
         virtual ~HeartbeatThread();

         /**
          * Realization of the PeriodicThread base class.
          * Overridden method of the parent, PeriodicThread, that defines the
          * action to take for the thread to do its work.
          */
         void PeriodicAsyncWorker();

         /**
          * force sending the system sending the heartbeat on request.
          */
         void sendHeartbeatNow();

      protected:

         /**
          * The access key for the device
          */
         string m_deviceAccessKey;

         /**
          * The Serial number of the device to send the heartbeat.
          */
         string m_deviceSerialNumber;

         /**
          * Communication queue used for sending message to the Server
          * and persisting data if communication network is down.
          */
         CommQueue* m_commQueue;

         /**
          * clock driver used to get the current time required for the heartbeat
          */
         IClockDriver* m_clockDriver;

         /**
          * Specific Device URL to send the Heartbeat.
          */
         string m_heartbeatURL;

         /**
          * The body of the HTTP request used to post a heartbeat.
          */
         string m_heartbeatBody;

         /**
          * Method to build up the message body of the heartbeat message.
          */
         void BuildHeartbeatBody();
   };
}

#endif
