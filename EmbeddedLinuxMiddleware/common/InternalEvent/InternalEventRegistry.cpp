
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <arpa/inet.h>

using namespace std;

#include "../../common/tinyxml/tinyxml2.h"
using namespace tinyxml2;

#include "../../common/basicTypes.h"
#include "../../common/util/EnumByName.h"
#include "../../common/DateTime/DateTime.h"
#include "../../common/util/BasicUtil.h"
#include "../../common/guid.h"

#include "../../common/logging/LogEntry.h"
#include "../../common/logging/ILogDriver.h"
#include "../../common/logging/emptyLogDriver.h"
#include "../../common/logging/ILoggable.h"
#include "../../common/logging/LogEntryEvents.h"

#include "../../common/CriticalSection/CriticalSection.h"

#include "../../common/LinkedList/SingleLink.h"
#include "../../common/LinkedList/SingleLinkedList.h"

#include "IInternalEvent.h"
#include "IInternalEventHandler.h"
#include "InternalEventRegistry.h"
#include "InternalEventRegistryEntry.h"
#include "GenericInternalEvent.h"
#include "SetConfigInternalEvent.h"


namespace CecilStLabs
{

   InternalEventRegistry::InternalEventRegistry()
      : m_maxfd(0),
        m_eventThreadRunning(false),
        m_eventThread(0),
        m_RegisteredEvents()
   {
      int ret;

      ret = pipe( m_fd );
      if( -1 == ret )
      {
         getLogDriver()->Log(std::string("InternalEventRegistry - open pipe error!!"), LoggingDebug );
      }
      // set the read description to nonblocking
      ret = fcntl(m_fd[0], F_SETFL, O_NONBLOCK);
      if( -1 == ret )
      {
         getLogDriver()->Log(std::string("InternalEventRegistry - fcntl error!!"), LoggingDebug );
      }

      // set max fd value + 1
      m_maxfd = m_fd[0] > m_fd[1] ? m_fd[0] : m_fd[1];
      m_maxfd++;

      BasicUtil::Assert((0 == pthread_mutex_init(&m_pipeSem, NULL)), __FILE__, __LINE__);
   }

   InternalEventRegistry::~InternalEventRegistry()
   {
      stop();

      BasicUtil::Assert((0 == pthread_mutex_destroy(&m_pipeSem)), __FILE__, __LINE__);
   }

   void InternalEventRegistry::start()
   {
      m_eventThreadRunning = true;;

      // start a thread to watch for  events
      pthread_create(&m_eventThread, NULL, &EventThread, this);
   }

   void InternalEventRegistry::stop()
   {
      m_eventThreadRunning = false;

      // wait for the thread to exit.
      pthread_join(m_eventThread, NULL);
   }

   bool InternalEventRegistry::sendEvent( IInternalEvent& event )
   {
      bool bret = true;
      int writeRet;

      if (MUTEX_SUCCESS == pthread_mutex_lock(&m_pipeSem))
      {
         // for size add in the size of the actual int holding size of data plus the size of the type
         writeRet = write(m_fd[1], event.getData(), event.getDataSize() + sizeof(int) + sizeof(eInternalEventType) );
         if( -1 ==  writeRet )
         {
            getLogDriver()->Log("InternalEvent - Event write error!!", LoggingDebug );

            bret = false;
         }
         pthread_mutex_unlock(&m_pipeSem);

      }
      return bret;
   }

   void InternalEventRegistry::registerEvent( eInternalEventType type, IInternalEventHandler* handler )
   {
      InternalEventRegistryEntry* evtPtr = new InternalEventRegistryEntry( type, handler );
      m_RegisteredEvents.insert( evtPtr, m_RegisteredEvents.last() );
   }

   void InternalEventRegistry::waitOnEvents()
   {
      int retVal;
      bool bret = false;
      IEVENT_DATA readEvent;

      fd_set rfds;
      struct timeval tv;

      // go to pipe to get more

      FD_ZERO(&rfds);
      FD_SET(m_fd[0], &rfds);

      tv.tv_sec = 1;
      tv.tv_usec = 0;

      retVal = select( m_maxfd, &rfds, NULL, NULL, &tv );
      if( -1 == retVal )
      {
         getLogDriver()->Log("InternalEvent - select error!!", LoggingDebug );
      }
      else if( retVal >= 0 )
      {
         // at least one event available
         memset( &readEvent, 0, sizeof(IEVENT_DATA) );
         bret = readFromPipe( readEvent );
         if( true == bret )
         {
            InternalEventRegistryEntry* entry = (InternalEventRegistryEntry*)m_RegisteredEvents.first();
            while( entry )
            {
               if( entry->getEventType() == readEvent.type )
               {
                  entry->getEventHandler()->processEvent( readEvent );
               }
               entry = (InternalEventRegistryEntry*)m_RegisteredEvents.next(entry);
            }
         }
      }
   }

   bool InternalEventRegistry::readFromPipe( IEVENT_DATA& event )
   {
      int readRet;
      bool bret = false;

      if (MUTEX_SUCCESS == pthread_mutex_lock(&m_pipeSem))
      {

         readRet = read( m_fd[0], &event.type, sizeof(eInternalEventType) );
         if( readRet == sizeof(eInternalEventType) )
         {

            // now get the size
            readRet = read( m_fd[0], &event.size, sizeof(int) );
            if( readRet == sizeof(int) )
            {
               // get rest of event based on size just read

               // just do a sanity check here to make sure no buffer overrun
               if( event.size <= (int)(MAX_EVENT_DATA) )
               {
                  // read into buffer
                  readRet = read( m_fd[0], &event.data, event.size );
                  if( readRet == event.size )
                  {
                     bret = true;
                  }

               }
               else
               {
                  getLogDriver()->Log("InternalEvent - trying to read too much data", LoggingDebug );
               }

            }

         }

         pthread_mutex_unlock(&m_pipeSem);

      }

      return bret;
   }

   void* EventThread(void* arg)
   {
      InternalEventRegistry* registry = (InternalEventRegistry*)arg;

      while (registry->isEventThreadRunning())
      {
         registry->waitOnEvents();
      }

      return NULL;
   }

}


