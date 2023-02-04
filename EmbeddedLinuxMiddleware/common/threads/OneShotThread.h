#ifndef ONESHOTTHREAD_H_
#define ONESHOTTHREAD_H_

namespace CecilStLabs
{
   /**
    * A task that is performed once after a set time
    */
   class OneShotThread
   {
      public:

         /**
          * Starts the thread running and doing the work defined by the
          * OneShotWorkerThread member function.
          *
          * @param delay number of seconds before performing task
          */
         virtual void start(uint16_t delay);


         /**
          * The work to be done once.  The worker function needs to be
          * defined by the child class that implements the periodic thread.
          */
         virtual void OneShotWorker() = 0;


         /**
          * Getter for delay
          */
         uint16_t getDelay() { return m_delay; }

      protected:
         /**
          * Constructor and destructor for the OneShotThread object.
          * these are protected to ensure this remains an abstract parent class
          * that must have a concrete child class implement the
          * PeriodicAsyncWorker method to define the specific Periodic Thread.
          */
         OneShotThread();
         virtual ~OneShotThread();

         /**
          * Define the default constructor, copy constructor, and assignment
          * operator as protected to ensure the compiler doesn't provide public
          * versions and ensures this abstract base class must be implemented
          * by a concrete child class for an instance of this object to exist.
          */
         OneShotThread(OneShotThread& copy __attribute__((unused)))
            : m_periodicThread(0)
         {
            // intentionally left blank.
         };

         OneShotThread& operator=(OneShotThread& rhs __attribute((unused))) { return rhs; };

         /**
          * The thread id
          */
         pthread_t m_periodicThread;

         /**
          * the delay to wait before doing the work, in seconds
          */
         uint16_t m_delay;

   };

   /**
    * Non-member function used for starting up the thread.
    * The pThread creation method requires a non-class member to startup.
    * This non-member function is passed an instance of the OneShotThread object
    * The passed object's member function ::OneShotWorkerThread() method is called
    * to perform the work.
    *
    * @param OneShotThread object to call the internal OneShotWorkerThread
    *                       worker method.
    */
   void* OneShotWorkerThread(void* arg);
}

#endif
