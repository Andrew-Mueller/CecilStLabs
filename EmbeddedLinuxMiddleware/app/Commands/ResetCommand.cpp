#include <errno.h>
#include <sys/reboot.h>

using namespace std;

#include "../../common/tersoTypes.h"               //lint !e761
#include "../../common/util/EnumByName.h"
#include "../../common/DateTime/DateTime.h"
#include "../../common/DateTime/IClockDriver.h"
#include "../../common/util/TersoUtil.h"
#include "../../common/guid.h"

#include "../../common/LinkedList/SingleLink.h"
#include "../../common/LinkedList/SingleLinkedList.h"
#include "../../common/MemoryPool/MemoryPool.h"

#include "../../common/tinyxml/tinyxml2.h"
using namespace tinyxml2;

#include "../../common/logging/LogEntry.h"
#include "../../common/logging/ILogDriver.h"
#include "../../common/logging/emptyLogDriver.h"
#include "../../common/logging/ILoggable.h"

#include "../../common/util/NameValuePair.h"
#include "../../common/InternalEvent/IInternalEvent.h"
#include "../../common/InternalEvent/IInternalEventHandler.h"
#include "../../common/InternalEvent/InternalEventRegistry.h"

#include "../jetstreamMessages.h"
#include "../Events/jetstreamEvent.h"
#include "../Events/CommandCompletionEvent.h"

#include "Command.h"
#include "ResetCommand.h"

namespace Terso
{
   ResetCommand::ResetCommand()
      //: m_cmdCompletionEvent("", "", NULL, new CommQueue(), new GUID())
   {
      // intentionally left blank
   }

   ResetCommand::~ResetCommand()
   {
      // intentionally left blank
   }

   CommandCompletionEvent* ResetCommand::action()
   {
      int status = 0;

      // TODO: queue the command completion event for the reset command.
      //       or does the command completion need to happen AFTER the system resets (this could be tricky)?

      /*
         <Jetstream xmlns="http://Jetstream.TersoSolutions.com/v1.0/CommandCompletionEvent">
            <!-- TODO: where does the event id come from? -->
            <Header EventId="5910078b-fc25-48c1-aee2-3744e3219a3d"
                    EventTime="2012-09-29T20:28:00Z"
                    ReceivedTime="2012-09-29T20:28:03Z"/>
            <!-- TODO: where does CommandID come from? -->
            <CommandCompletionEvent CommandId="0234fd0e-bdd7-41e7-8abb-6a728e401aa3">
               <OutputParameterList/>
               <ExceptionList/>
               <DeviceExtensionList />
            </CommandCompletionEvent>
         </Jetstream>
       */

      // TODO: add syslog that a reboot has been requested

      #ifndef __APPLE__
         // reset the system.
         status = reboot(LINUX_REBOOT_CMD_RESTART);
      #else
         cout << "DEBUG - REBOOT" << endl;
      #endif

      if (0 != status)
      {
         if (EINVAL == errno)
         {
            // TODO: log "bad magic number"
         }

         if (EPERM == errno)
         {
            // TODO: log "the user must be root"
         }
      }

      // TODO: we do want to return the command completion event, ideally so that
      //       it gets enqueued before the reset, and then sent out AFTER the reset.

      return &m_cmdCompletionEvent;
   }
}
