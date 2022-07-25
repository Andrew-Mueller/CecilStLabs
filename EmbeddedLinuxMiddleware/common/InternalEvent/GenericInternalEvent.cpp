
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>


#include <cstdio>
#include <cstring>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <arpa/inet.h>

#include "IInternalEvent.h"
#include "GenericInternalEvent.h"

namespace CecilStLabs
{
  
   GenericInternalEvent::GenericInternalEvent()
   {
      m_data.type = IEVT_GENERIC; 
      m_data.size = 0; 
      memset( m_data.data, 0, MAX_EVENT_DATA );
   }

   GenericInternalEvent::~GenericInternalEvent()
   {
      // intentionally left blank
   }
}
