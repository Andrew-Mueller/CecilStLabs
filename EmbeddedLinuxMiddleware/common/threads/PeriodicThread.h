#ifndef PERIODICTHREAD_H_
#define PERIODICTHREAD_H_

namespace CecilStLabs
{
   /**
    * Enumeration of the possible resolutions of how often the periodic thread
    * runs.
    */
   enum ePeriodInterval
   {
      ePeriodInterval_Minutes        = 060,  //
      ePeriodInterval_Seconds        = 001,  // 1
      ePeriodInterval_Milliseconds   = 103   // 10 e -3 = 0.001
   };

   /**
    * A task that is performed at a pre-set number of minutes.
    */
   class PeriodicThread
   {
      public:

         /**
          * Starts the thread running and doing the work defined by the
          * PeriodicAsyncWorker member function.
          */
         virtual void Start();

         /**
          * Stops the thread from running.  The thread can be restarted by
          * calling the Start method.
          *
          * NOTE: no resources are released, the thread just stops executing its
          *       worker function.
          */
         virtual void Stop();

         /**
          * Gets the running state of the thread.
          *
          * @returns True if the thread is running, False if it is not.
          */
         bool IsRunning();

         /**
          * Retrieves the number of minutes the thread waits between the
          * periodic thread doing its work (calling its PeriodicAsyncWorker
          * method.
          *
          * @return Minutes between performing the periodic thread's work.
          */
         uint16_t GetPeriod();

         /**
          * Retrieves the enumeration of how to interpret the value returned from GetPeriod
          * as nanoseconds, milliseconds, seconds, or minutes
          *
          * @return Enumeration representing the units of the GetPeriod value.
          */
         ePeriodInterval GetPeriodInterval();

         /**
          * Sets the number of minutes that the periodic thread waits between
          * calling its periodic async worker method.
          *
          * NOTE: The thread will restart from minute zero with the new number
          *       of minutes.
          *
          * @param period The new number of seconds the thread will wait between
          *               doing its work via its PeriodicAsyncWofrker function.
          */
         virtual void SetPeriod(const uint16_t period, const ePeriodInterval periodInterval);

         /**
          * The work to be done periodically.  The worker function needs to be
          * defined by the child class that implements the periodic thread.
          */
         virtual void PeriodicAsyncWorker() = 0;

         /**
          * TODO: this may make more sense in the basicTypes or limits header.
          *       for now, this is the only place it is used, so this is where
          *       it will go.
          */
         static const uint64_t NANO_SECONDS_PER_SECOND = 1000000000LL;

      protected:
         /**
          * Constructor and destructor for the PeriodicThread object.
          * these are protected to ensure this remains an abstract parent class
          * that must have a concrete child class implement the
          * PeriodicAsyncWorker method to define the specific Periodic Thread.
          */
         PeriodicThread(const uint16_t period, const ePeriodInterval periodInterval);
         virtual ~PeriodicThread();

         /**
          * Define the default constructor, copy constructor, and assignment
          * operator as protected to ensure the compiler doesn't provide public
          * versions and ensures this abstract base class must be implemented
          * by a concrete child class for an instance of this object to exist.
          */
         PeriodicThread() : m_period(1), m_periodInterval(ePeriodInterval_Minutes), m_running(false), m_periodicThread(0)
         {
            // intentionally left blank.
         };

         PeriodicThread(PeriodicThread& copy __attribute__((unused)))
            : m_period(copy.m_period),
              m_periodInterval(copy.m_periodInterval),
              m_running(false),
              m_periodicThread(0)
         {
            // intentionally left blank.
         };

         PeriodicThread& operator=(PeriodicThread& rhs __attribute((unused))) { return rhs; };

         /**
          * The number of minutes to wait between performing the work defined
          * in the PeriodicAsyncWorker method.
          */
         uint16_t m_period;

         /**
          * how often to run the periodic thread should run.
          */
         ePeriodInterval m_periodInterval;

         /**
          * flag to inform a caller that the thread is actively running or stopped.
          */
         bool m_running;

         /**
          * The thread for periodically sending the heart beat.
          */
         pthread_t m_periodicThread;

         /**
          * Semaphore protection data utilized by the thread.
          */
         pthread_mutex_t m_periodicSemaphore;
   };

   /**
    * Non-member function used for starting up the thread.
    * The pThread creation method requires a non-class member to startup.
    * This non-member function is passed an instance of the PeriodicThread object
    * The passed object's member function ::PeriodicAsyncWorker() method is called
    * to perform the work.
    *
    * @param PeriodicThread object to call the internal PeriodicAsyncWorker
    *                       worker method.
    */
   void* PeriodicWorkerThread(void* arg);
}

#endif
