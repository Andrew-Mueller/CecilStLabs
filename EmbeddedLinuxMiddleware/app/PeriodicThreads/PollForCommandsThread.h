#ifndef POLLFORCOMMANDSTHREAD_H
#define POLLFORCOMMANDSTHREAD_H

namespace CecilStLabs
{
   class PollForCommandsThread : public PeriodicThread, public ILoggable
   {
      public:
         /**
          * Create the Thread to periodically poll for commands with the
          * specified device access data.
          * and number of seconds to define the heartbeat's period.
          * @param deviceAccessKey The guid that provides access ot post
          *        messages for the device.
          * @param deviceURL The Device Web Server URL to connect to.
          * @param pollCommand_seconds The number of seconds between polling for
          *        new commands.
          * @param session The SSL information used to communicate with the server.
          */
         PollForCommandsThread(std::string deviceAccessKey,
                               CommQueue& commQueue,
                               uint16_t pollCommand_seconds);

         /**
          * Release any resources used (currently none).
          */
         virtual ~PollForCommandsThread();

         /**
          * Overridden method of the parent, PeriodicThread, that defines the
          * action to take for the thread to do its work.
          */
         void PeriodicAsyncWorker();

      protected:

         /**
          * Communication queue used for sending message to the Server
          * and persisting data if communication network is down.
          */
         CommQueue& m_commQueue;

         /**
          * Specific Device URL to send the Poll for commands message.
          */
         std::string m_getCommandsURL;

         /**
          * The access key for the device
          */
         std::string m_deviceAccessKey;
   };
}

#endif
