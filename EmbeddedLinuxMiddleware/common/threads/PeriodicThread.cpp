#include <unistd.h>
#include <stdlib.h>
#include <time.h>

//#include <eventfd.h>
//#include <poll.h>

#include <errno.h>

#include <iostream>
using namespace std;

#include <pthread.h>

#include "../basicTypes.h"
#include "../util/EnumByName.h"
#include "../DateTime/DateTime.h"

#include "../logging/LogEntry.h"
#include "../logging/ILogDriver.h"

#include "../util/BasicUtil.h"

#include "PeriodicThread.h"

// NOTE: there is a difference in the pthread library that is packaged with
//       Mac OSX and the latest version of pthread available.  Until the pthread
//       library can be updated on the development environment, this is a
//       workaround.
#ifdef __APPLE__
extern void pthread_yield();
#endif

#define PERIODIC_THREAD_DEBUG_PRINT

namespace CecilStLabs
{
   void PeriodicThread::Start()
   {
      if( m_period > 0 )
      {
         if (MUTEX_SUCCESS == pthread_mutex_lock(&m_periodicSemaphore))
         {
            m_running = true;

            pthread_mutex_unlock(&m_periodicSemaphore);
         }

         int status = 0;
         status = pthread_create(&m_periodicThread, NULL, &PeriodicWorkerThread, this);

         BasicUtil::Assert((0 == status), __FILE__, __LINE__);
      }
   }

   void PeriodicThread::Stop()
   {
      if (MUTEX_SUCCESS == pthread_mutex_lock(&m_periodicSemaphore))
      {
         // set the flag to false so the thread will stop iterating.
         m_running = false;

         pthread_mutex_unlock(&m_periodicSemaphore);
      }

      // wait for the periodic thread to exit (only if the thread had been created).
      if (0 != m_periodicThread)
      {
         pthread_join(m_periodicThread, NULL);
      }
   }

   bool PeriodicThread::IsRunning()
   {
      bool running = false;

      if (MUTEX_SUCCESS == pthread_mutex_lock(&m_periodicSemaphore))
      {
         running = m_running;

         pthread_mutex_unlock(&m_periodicSemaphore);
      }

      return running;
   }

   uint16_t PeriodicThread::GetPeriod()
   {
      uint16_t period = 0;

      if (MUTEX_SUCCESS == pthread_mutex_lock(&m_periodicSemaphore))
      {
         period = m_period;

         pthread_mutex_unlock(&m_periodicSemaphore);
      }

      return period;
   }

   ePeriodInterval PeriodicThread::GetPeriodInterval()
   {
      ePeriodInterval periodInterval = (ePeriodInterval)0;

      if (MUTEX_SUCCESS == pthread_mutex_lock(&m_periodicSemaphore))
      {
         periodInterval= m_periodInterval;

         pthread_mutex_unlock(&m_periodicSemaphore);
      }

      return periodInterval;
   }

   void PeriodicThread::SetPeriod(const uint16_t period, const ePeriodInterval periodInterval)
   {
      // stop it, set new value for the period, and then restart.
      Stop();

      if (MUTEX_SUCCESS == pthread_mutex_lock(&m_periodicSemaphore))
      {
         m_period = period;
         m_periodInterval = periodInterval;

         pthread_mutex_unlock(&m_periodicSemaphore);
      }

      Start();
   }

   PeriodicThread::PeriodicThread(const uint16_t period, const ePeriodInterval periodInterval)
      : m_period(period),
        m_periodInterval(periodInterval),
        m_running(false),
        m_periodicThread(0)
   {
      BasicUtil::Assert((0 == pthread_mutex_init(&m_periodicSemaphore, NULL)), __FILE__, __LINE__);
   }

   PeriodicThread::~PeriodicThread()
   {
      // make sure the thread is stopped and dispose of the pthread

      Stop();

      pthread_mutex_destroy(&m_periodicSemaphore);
   }

   void* PeriodicWorkerThread(void* arg)
   {
      // when this function exits, release the threads
      // resources without joining.

      if (NULL != arg)
      {
         PeriodicThread* periodicArg = (PeriodicThread*)arg;

         while (periodicArg->IsRunning())
         {
            struct timespec now;

            // perform the thread's requested work.
            periodicArg->PeriodicAsyncWorker();

            // NOTE: CLOCK_MONOTONIC is the total number of seconds since the system
            // started, until it overflows the datatype (uint32_t), and then just
            // continues to roll.
            clock_gettime(CLOCK_MONOTONIC, &now);

            struct timespec period;

            // calculate this based on the member periodic interval.
            // using lastRun to subtract the proper number of nano seconds from the period
            if (ePeriodInterval_Minutes == periodicArg->GetPeriodInterval())
            {
               period.tv_sec = (periodicArg->GetPeriod() * 60) + now.tv_sec;
               period.tv_nsec = now.tv_nsec;
            }
            else if (ePeriodInterval_Seconds == periodicArg->GetPeriodInterval())
            {
               period.tv_sec = periodicArg->GetPeriod() + now.tv_sec;
               period.tv_nsec = now.tv_nsec;
            }
            else if (ePeriodInterval_Milliseconds == periodicArg->GetPeriodInterval())
            {
               period.tv_sec = now.tv_sec;
               period.tv_nsec = (periodicArg->GetPeriod() * 1000000) + now.tv_nsec;
            }
            else
            {
               // unknown interval
               // log the error so it is known


               period.tv_sec = now.tv_sec;
               period.tv_nsec = now.tv_nsec + 1;
            }

            // finally, let the thread sleep for the specified interval before
            // running again.
            // The Absolute timer is used to help avoid error buildup resulting
            // in all of the thread deadlines drifting.
            if (0 != clock_nanosleep(CLOCK_MONOTONIC,
                                     TIMER_ABSTIME,
                                     &period,
                                     NULL))
            {
               // sleep was interrupted, or an error occured while attempting to sleep.

            }
         }
      }

      return NULL;
   }
}
