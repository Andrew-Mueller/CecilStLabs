#include <pthread.h>                            //lint !e537
#include <unistd.h>
#include <sys/time.h>

#include "../basicTypes.h"
#include "../util/BasicUtil.h"

#include "../LinkedList/SingleLink.h"
#include "../LinkedList/SingleLinkedList.h"

#include "ScheduledThread.h"

namespace CecilStLabs
{
   ScheduledThread::ScheduledThread()
      : m_taskList(),
        m_thread(0),
        m_isRunning(false)
   {

      // get the current system hour.
      UpdateHour();
   }

   ScheduledThread::~ScheduledThread()
   {
      // intentionally left blank.
   }

   void ScheduledThread::UpdateHour()
   {
      // wait for an hour then take a look at each task listed.
      timeval time;
      gettimeofday(&time, NULL);

      uint8_t secsToWait = time.tv_sec % 60;

      m_currentHour = time.tv_sec / 60;

      sleep(secsToWait);
   }

   void ScheduledThread::Start()
   {
      m_isRunning = true;

      // setup the thread
      int status = 0;
      status = pthread_create(&m_thread, NULL, &ScheduledWorkerThread, this);

      BasicUtil::Assert((0 == status), __FILE__, __LINE__);
   }

   void ScheduledThread::Stop()
   {
      // set the flag to false so the thread will stop iterating.
      m_isRunning = false;

      // wait for the periodic thread to exit.
      pthread_join(m_thread, NULL);
   }

   void ScheduledThread::ScheduledAsyncWorker()
   {
      // roll through the list of scheduled tasks and execute the tasks
      // for the current hour.
      ScheduledTask* task = (ScheduledTask*)m_taskList.first();

      while (NULL != task)
      {
         if ((m_currentHour == task->GetHour()) &&
             (!task->getRanToday()))
         {
            task->setRanToday();
            task->Task();
         }

         task = (ScheduledTask*)m_taskList.next(task);
      }
   }

   void ScheduledThread::Add(ScheduledTask* task)
   {
      m_taskList.insert(task, NULL);
   }

   void* ScheduledWorkerThread(void* arg)
   {
      if (NULL != arg)
      {
         ScheduledThread* sched_thread = (ScheduledThread*)arg;

         while (sched_thread->IsRunning())
         {
            sched_thread->ScheduledAsyncWorker();
         }

         sched_thread->UpdateHour();
      }

      return NULL;
   }
}
