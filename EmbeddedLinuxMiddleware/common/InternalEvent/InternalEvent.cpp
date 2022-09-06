
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

#include "../../common/CriticalSection/CriticalSection.h"

#include "../../common/LinkedList/SingleLink.h"
#include "../../common/LinkedList/SingleLinkedList.h"

#include "SingleEvent.h"
#include "InternalEvent.h"


namespace CecilStLabs
{
  
   InternalEvent::InternalEvent()
   {
      int ret;
      
      ret = pipe( m_fd );
      if( -1 == ret )
      {
         // how would this even happend?
         cout << "InternalEvent - open pipe error!!" << endl;
      }
      // set the read descriptoe to nonblocking
      ret = fcntl(m_fd[0], F_SETFL, O_NONBLOCK);
      if( -1 == ret )
      {
         // how would this even happend?
         cout << "InternalEvent - fcntl error!!" << endl;
      }   
      
      // set max fd value + 1
      m_maxfd = m_fd[0]>m_fd[1] ? m_fd[0]:m_fd[1];
      m_maxfd++;
   }

   InternalEvent::~InternalEvent()
   {
      // intentionally left blank
   }

   bool InternalEvent::SendEvent( const eEventType type, const char* data, const int size ) 
   {
      bool bret = true;
      int writeRet;
      EVENT_DATA evtData;
      
      if( size > MAX_EVENT_DATA )
      {
         bret = false;
      }
      else
      {
         // total size of structure with data
         evtData.size = size + sizeof(eEventType) + sizeof(int);
         evtData.type = type;
         
         if( size > 0 && data != NULL )
         {
             memcpy( evtData.data, data, size );
         }
         m_criticalSection.Enter();
         writeRet = write(m_fd[1], &evtData, evtData.size );
         m_criticalSection.Exit();
         if( -1 ==  writeRet )
         {
            // How would this even happen?
            cout << "InternalEvent - Event write error!!" << endl;
            
            bret = false;
         }
      }
      
      return bret;
            
   }
         
   eEventType InternalEvent::WaitOnEvent( char* data, int* size, const int timeout )  
   {
      eEventType retEvt = EVT_NONE;
      int retVal;
      
      fd_set rfds;
      struct timeval tv;
      struct timeval* tv_ptr;
      
      SingleEvent* retSingle = (SingleEvent*)m_Events.remove();
      if( retSingle != NULL )
      {
         // there is already an event available, return it
         retEvt = retSingle->getEventType();
         if( NULL != data && NULL != size && size > 0 )
         {
            // watch buffer size
            if( *size > retSingle->getDataSize()+1 )
            {
                memcpy( data, retSingle->getData(), retSingle->getDataSize() );
                data[retSingle->getDataSize()] = 0;
                // include NULL in size
                *size = retSingle->getDataSize()+1;
            }
            else
            {
               // truncate data
               memcpy( data, retSingle->getData(), *size-1 );
               data[*size-1] = 0;
            }
         }
         
         // delete item!
         delete retSingle;
      }
      else
      {
         // go to pipe to get more
            
         FD_ZERO(&rfds);
         FD_SET(m_fd[0], &rfds);

         // if -1 was passed in, send NULL insot select for infinite timeout
         if( -1 == timeout )
         {
            tv_ptr = NULL;
         }
         else
         {
            tv.tv_sec = timeout/1000;
            tv.tv_usec = (timeout%1000)*1000;
            tv_ptr = &tv;
         }

         retVal = select( m_maxfd, &rfds, NULL, NULL, tv_ptr );
         if( -1 == retVal )
         {
            // How would this happen??
            cout << "InternalEvent - select error!! " << errno << endl;
         }
         else if( retVal >= 0 )
         {
            // at least one event available
            retEvt = ReadFromPipe( data, size );
         }
               
      }
      return retEvt;
      
   }
   
   eEventType InternalEvent::ReadFromPipe( char* data, int* size ) 
   {
      eEventType retEvt = EVT_NONE;
      int readRet;
      int dataSize;
      int evtCnt = 0;
    
      int tmpSize; 
      EVENT_DATA tmpEvt;
      memset( &tmpEvt, 0, sizeof(tmpEvt) );
      
      m_criticalSection.Enter();
      
      while(1)
      {
         readRet = read( m_fd[0], &tmpSize, sizeof(int) );
         if( readRet == sizeof(int) )
         {
            tmpEvt.size = tmpSize;
             // get rest of event
            // note because we already read the size, the start is AFTER the size member,
            //  and tmpEvt the size to read is minus the size of the size (int)
            tmpSize = tmpSize - sizeof(int);
                  
            // just do a sanity check here to make sure no buffer overrun
            if( tmpSize <= (int)(MAX_EVENT_DATA+sizeof(eEventType)) )
            {   
               // read into buffer starting at type, the data is after it so it will just keep filling
               readRet = read( m_fd[0], &tmpEvt.type, tmpSize );
               dataSize = readRet - sizeof(eEventType);
            }
            else
            {
               // How would this happen??
               cout << "InternalEvent - trying to read too much data" << endl;
               break;
            }
               
            if( readRet >= (int)sizeof(eEventType) )
            {
               if( 0 == evtCnt )
               {
                  //first one read, return it
                  retEvt = tmpEvt.type;
                  if( NULL != data && NULL != size && size > 0 )
                  {
                     // watch buffer size and NULL terminate
                     if( *size > dataSize )
                     {
                         memcpy( data, tmpEvt.data, dataSize );
                         data[dataSize] = 0;
                         // include NULL in size
                         *size = dataSize+1;
                     }
                     else
                     {
                        // truncate data
                        memcpy( data, tmpEvt.data, *size-1 );
                        data[*size-1] = 0;                         
                     }
                  }
               }
               else
               {
                  // queue it
                  SingleEvent* evtPtr = new SingleEvent( tmpEvt.type, dataSize, tmpEvt.data );
                  m_Events.insert( evtPtr, m_Events.last() );
               }
                        
            }
            else
            {
               break;
            }
         }
         else
         {
            // read failed, no more data
            break;
         }
         
         m_criticalSection.Exit();
       
         evtCnt++;
      }
      
      return retEvt;
   }

 
}


