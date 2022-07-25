#include <iostream>
using namespace std;

#include <pthread.h>
#include <errno.h>

#include "CriticalSection.h"

namespace CecilStLabs
{
   CriticalSection::CriticalSection()
   {
      /*
      pthread_mutexattr_t m_critSection_mutexattr;

      // set the mutex as a recursive mutex
      // TODO: Àwhat does a recursive mutex mean?
      pthread_mutexattr_settype(&m_critSection_mutexattr, PTHREAD_MUTEX_RECURSIVE);

      // create the mutex with the attributes set
      pthread_mutex_init(&m_critSection_mutex, &m_critSection_mutexattr);

      // The mutex is created and the attributes have served their purpose.
      pthread_mutexattr_destroy(&m_critSection_mutexattr);
      */
   }

   CriticalSection::~CriticalSection()
   {
      // release any waiting resources.  I would expect none to be waiting by the
      // time we call the destructor.
      //pthread_mutex_unlock(&m_critSection_mutex);

      // destroy the mutex, releasing the mutex resources.
      //pthread_mutex_destroy(&m_critSection_mutex);
   }

   void CriticalSection::Enter()
   {
      /*
      if (0 == pthread_mutex_lock(&m_critSection_mutex))
      {
         #ifdef DEBUG_PRINT
            cout << "mutex locked" << endl;
         #endif
      }
      else
      {
         #ifdef DEBUG_PRINT
            cout << "error locking the mutex " << errno << endl;
         #endif
      }
      */
   }

   void CriticalSection::Exit()
   {
      /*
      if (0 == pthread_mutex_unlock(&m_critSection_mutex))
      {
         #ifdef DEBUG_PRINT
            cout << "mutex unlocked" << endl;
         #endif
      }
      else
      {
         #ifdef DEBUG_PRINT
            cout << "error unlocking the mutex " << errno << endl;
         #endif
      }
      */
   }
}
