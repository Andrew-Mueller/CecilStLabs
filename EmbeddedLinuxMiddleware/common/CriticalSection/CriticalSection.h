#ifndef CRITICALSECTION_H
#define CRITICALSECTION_H

#include <pthread.h>

namespace CecilStLabs
{
   /**
    * Critical Section object to abstract the whatever platform specific concept
    * is used to perform controlled entry into a critical section.
    */
   class CriticalSection
   {
      public:
         CriticalSection();
         virtual ~CriticalSection();

         /**
          * Enters the critical section. Blocks until critical section can be entered.
          */
         void Enter();

         /**
          * Exits the critical section.
          */
         void Exit();

      protected:

         /**
          * mutex used for the enter/exit critical section functions.
          */
         pthread_mutex_t m_critSection_mutex;
   };
}

#endif
