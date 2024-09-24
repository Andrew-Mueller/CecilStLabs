#include <errno.h>
#include <sys/reboot.h>

using namespace std;

#include "../../common/basicTypes.h"               //lint !e761
#include "../../common/util/EnumByName.h"
#include "../../common/DateTime/DateTime.h"
#include "../../common/DateTime/IClockDriver.h"
#include "../../common/util/BasicUtil.h"
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

#include "../Messages.h"
#include "../Events/Event.h"
#include "../Events/CommandCompletionEvent.h"

#include "Command.h"
#include "ResetCommand.h"

namespace CecilStLabs
{
   ResetCommand::ResetCommand()
      : m_cmdCompletionEvent("", "", NULL, new CommQueue(), new GUID())
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
      //       or does the command completion need to happen AFTER the system
      //       resets (this could be tricky)?

      // TODO: add syslog that a reboot has been requested

      #ifndef __APPLE__
         // reset the system.
         status = reboot(LINUX_REBOOT_CMD_RESTART);
      #else
         // for development and debugging.
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
