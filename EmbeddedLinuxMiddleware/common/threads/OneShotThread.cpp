#include <unistd.h>
#include <stdlib.h>
#include <time.h>

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

#include "OneShotThread.h"

// NOTE: there is a difference in the pthread library that is packaged with
//       Mac OSX and the latest version of pthread available.  Until the pthread
//       library can be updated on the development environment, this is a
//       workaround.
#ifdef __APPLE__
extern void pthread_yield();
#endif


namespace CecilStLabs
{
   void OneShotThread::start( uint16_t delay )
   {
      m_delay = delay;

      int status = 0;
      status = pthread_create(&m_periodicThread, NULL, &OneShotWorkerThread, this);

      BasicUtil::Assert((0 == status), __FILE__, __LINE__);
   }


   OneShotThread::OneShotThread()
      : m_periodicThread(0)
   {

   }

   OneShotThread::~OneShotThread()
   {
   }

   void* OneShotWorkerThread(void* arg)
   {
      // when this function exits, release the threads
      // resources without joining.

      if (NULL != arg)
      {
         OneShotThread* oneShotArg = (OneShotThread*)arg;

         if( 0 < oneShotArg->getDelay() )
         {
            sleep( oneShotArg->getDelay() );
         }

          // perform the thread's requested work.
         oneShotArg->OneShotWorker();
      }

      return NULL;
   }
}
