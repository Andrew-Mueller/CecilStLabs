#ifndef SCHEDULEDTHREAD_H
#define SCHEDULEDTHREAD_H

namespace CecilStLabs
{
   /**
    * Task to be added to the ScheduledThread.
    *
    * Each item that wants to be scheduled by the ScheduleThread needs to
    * inherit from the ScheduledTask object.
    */
   class ScheduledTask : public SingleLink
   {
      public:
         ScheduledTask(uint8_t hour) : m_hour(0), m_ranToday(false)
         {
            // utilize the error checking.
            SetHour(hour);
         };
         virtual ~ScheduledTask() {};

         /**
          * Getter for the Hour to execute the Task specified.
          */
         inline uint8_t GetHour() { return m_hour; };

         /**
          * Setter for the Hour to execute the Task specified.
          */
         void SetHour(uint8_t hour)
         {
            if (hour < MAX_HOUR)
            {
               m_hour = hour;
            }
         }

         /**
          * Accessors to see if this task has already run today, and to set
          * the daily run
          */
         inline bool getRanToday() { return m_ranToday; };
         inline void setRanToday() { m_ranToday = true; };

         /**
          * Task containing the work to be performed at the specified hour.
          */
         virtual void Task() = 0;

      protected:
         static const uint8_t MAX_HOUR = 24;

         /**
          * Hour where the scheduled task should be executed.
          */
         uint8_t m_hour;

         /**
          * flag if the task has already run today to ensure it only runs once
          * per day (on the specified hour).  It is reset every day.
          */
         bool m_ranToday;
   };

   /**
    * List of in-frequent tasks to execute.  These are things that occur about
    * once a day.
    */
   class ScheduledThread
   {
      public:
         ScheduledThread();
         virtual ~ScheduledThread();

         /**
          * Getter for the running state of the state.
          */
         bool IsRunning() { return m_isRunning; };

         /**
          * Starts the scheduled thread worker.
          */
         void Start();

         /**
          * Stops the scheduled thread worker.
          */
         void Stop();

         /**
          * Add the specified task to this hourly scheduler.
          */
         void Add(ScheduledTask* task);

         /**
          * Update the current hour for the scheduler.
          */
         void UpdateHour();

         /**
          * Worker function used for checking tasks if it is their time to run.
          */
         void ScheduledAsyncWorker();

      protected:

         /**
          * list of tasks to execute.
          */
         SingleLinkedList m_taskList;

         /**
          * Thread in charge of checking for scheduled tasks.
          */
         pthread_t m_thread;

         /**
          * stores the running status of the thread.  Used to flag when the work is done.
          */
         bool m_isRunning;

         /**
          * The current hour for the scheduler
          */
         uint8_t m_currentHour;
   };

   /**
    * Non-member function used for starting up the thread.
    * The pThread creation method requires a non-class member to startup.  This
    * non-member function is passed an instance of the PeriodicThread object
    * The passed object's member function ::PeriodicAsyncWorker() method is called
    * to perform the work.
    *
    * @param ScheduledThread object to call the internal Scheduled worker
    */
   void* ScheduledWorkerThread(void* arg);

}
#endif
