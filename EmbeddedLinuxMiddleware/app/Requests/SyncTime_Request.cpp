
#include <iostream>
#include <sstream>
#include <string>
using namespace std;

#include <sys/socket.h>
#include <resolv.h>
#include <netdb.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#include "../../common/basicTypes.h"               //lint !e761
#include "../../common/util/EnumByName.h"
#include "../../common/DateTime/DateTime.h"
#include "../../common/util/BasicUtil.h"
#include "../../common/guid.h"


#include "../../common/logging/LogEntry.h"
#include "../../common/logging/ILogDriver.h"
#include "../../common/logging/emptyLogDriver.h"
#include "../../common/logging/ILoggable.h"

#include "../../common/LinkedList/SingleLink.h"
#include "../../common/LinkedList/SingleLinkedList.h"

#include "../../common/threads/ScheduledThread.h"

#include "../../common/DateTime/IClockDriver.h"

#include "../../common/RingBuffer/RingBuffer.h"
#include "../../common/MemoryPool/MemoryPool.h"

#include "../../comms/RXData/RXData.h"
#include "../../comms/SocketError.h"
#include "../../comms/SocketBase.h"
#include "../../comms/Socket.h"
#include "../../comms/SecureSession.h"
#include "../../comms/SecureSocket.h"

#include "../../comms/ICommProtocolResponse.h"
#include "../../comms/ICommProtocol.h"
#include "../../comms/HTTP/HTTPResponse.h"
#include "../../comms/HTTP/HTTPS.h"

#include "../../common/stateMachine/IState.h"
#include "../../common/stateMachine/Transition.h"
#include "../../common/stateMachine/ITransitionHandler.h"
#include "../../common/stateMachine/State.h"
#include "../../common/stateMachine/StateMachine.h"

#include "../../comms/CommRecord.h"
#include "../../comms/CommDAL.h"

#include "../../common/tinyxml/tinyxml2.h"
using namespace tinyxml2;

#include "../../common/config/IConfig.h"

#include "../../common/util/NameValuePair.h"
#include "../../common/InternalEvent/IInternalEvent.h"
#include "../../common/InternalEvent/IInternalEventHandler.h"
#include "../../common/InternalEvent/InternalEventRegistry.h"

#include "../Messages.h"
#include "../Events/Event.h"
#include "../Events/CommandCompletionEvent.h"

#include "../Commands/Command.h"
#include "../Commands/ResetCommand.h"
#include "../Commands/GetConfigValuesCommand.h"
#include "../Commands/SetConfigValuesCommand.h"
#include "../Commands/Commands.h"
#include "../Commands/CommandList.h"

#include "../MessageParser.h"

#include "../../comms/CommStateMachine/CommsSignals.h"
#include "../../comms/CommStateMachine/BidirectionalCommsAvailableState.h"
#include "../../comms/CommStateMachine/UnidirectionalCommsAvailableState.h"
#include "../../comms/CommStateMachine/CommsUnavailableState.h"

#include "../../comms/CommQueue.h"

#include "SyncTime_Request.h"

#undef SYNCTIME_DEBUG_PRINT

namespace CecilStLabs
{
   const std::string SyncTime_Request::m_unsecuredSyncTimeURL = "http://www.CecilStLabs.com/getdatetime";

   SyncTime_Request::SyncTime_Request(IClockDriver* clockDriver,
                                      CommQueue& commQueue,
                                      string deviceAccessKey,
                                      uint8_t hour)
      : ScheduledTask(hour),
        m_clockDriver(clockDriver),
        m_commQueue(commQueue),
        m_securedSyncTimeURL(""),
        m_deviceAccessKey(deviceAccessKey)
   {
      // build up the Poll for Commands URL
      ostringstream urlBuilder;
      urlBuilder << "https://" << commQueue.getJetstreamURL()
                 << "/v1.0/device/GetDateTime?AccessKey="
                 << m_deviceAccessKey;

      m_securedSyncTimeURL = urlBuilder.str();
   }

   SyncTime_Request::~SyncTime_Request()
   {
      // intentionally left blank
   }

   bool SyncTime_Request::Request()
   {
      bool success = false;

      getLogDriver()->Log("Requesting time sync...", LoggingDebug);

      CommRecord record(POST,
                        m_securedSyncTimeURL,
                        "",
                        eSynchTime_Msg,
                        m_clockDriver->getCurrentTime());

      m_commQueue.enqueue(record);

      // TODO: fake it for now.  The error from the queueing system must be
      //       plumbed back to this point
      success = true;

      return success;
   }

   void SyncTime_Request::Task()
   {
      if (!Request())
      {
         getLogDriver()->log(std::string("Error attempting to sync to the system clock to the server"), LoggingError);
      }
   }
}
