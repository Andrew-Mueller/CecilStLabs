
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

using namespace std;

#include "../../common/LinkedList/SingleLink.h"

#include "IInternalEvent.h"
#include "IInternalEventHandler.h"
#include "InternalEventRegistryEntry.h"


namespace CecilStLabs
{
   InternalEventRegistryEntry::InternalEventRegistryEntry( eInternalEventType type, IInternalEventHandler* handler ) :
      m_eventType(type), m_eventHandler(handler)
   {
      // intentionally left blank
   }

   InternalEventRegistryEntry::~InternalEventRegistryEntry()
   {
      // intentionally left blank
   }
}
