#ifndef COMMQUEUETHREAD_H_
#define COMMQUEUETHREAD_H_

namespace CecilStLabs
{

   /**
    * The Communication Queue thread that is a periodic checker for messages
    * that should be sent out to the Server.
    */
   class CommQueueThread : public PeriodicThread
   {
      public:

         /**
          * Create the Communication Queuing thread to manage and maintain messages
          * that are attempted to send to the server.
          *
          * @param The communication queue used for sending data to the server.
          * @param commandQueuePoll_minutes The number of minutes to wait between
          *                                 checking for queued messages.
          */
         CommQueueThread(CommQueue* queue,
                         uint16_t commandQueuePoll_minutes);

         /**
          * release the resources consumed by the communication queue thread.
          */
         virtual ~CommQueueThread();

         /**
          * PeriodicThread's worker method that performs the work to check the
          * communication's queue for any pending communications to attempt.
          */
         void PeriodicAsyncWorker();

      protected:

         /**
          * access to the queue of pending communication messages.
          */
         CommQueue* m_commQueue;
   };

}
#endif
