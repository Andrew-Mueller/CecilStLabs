
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>


#include <cstdio>
#include <cstring>
#include <cerrno>

using namespace std;

#include "../../common/LinkedList/SingleLink.h"

#include "SingleEvent.h"


namespace CecilStLabs
{
  
   SingleEvent::SingleEvent( const eEventType type, const int size, const char* data ) :
      eventType(type), dataSize(size)
   {
      memcpy( eventData, data, size );
      totalSize = size + sizeof(int) + sizeof(eEventType);
   }

   SingleEvent::~SingleEvent()
   {
      // intentionally left blank
   }

 
}


