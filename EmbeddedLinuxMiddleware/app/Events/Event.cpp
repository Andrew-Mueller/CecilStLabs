#include <cstring>

//lint -esym(18,*)
#include <iostream>
#include <sstream>
using namespace std;

#include <sys/socket.h>
#include <resolv.h>
#include <netdb.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#include "../../common/basicTypes.h"
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
#include "../../common/RingBuffer/RingBuffer.h"
#include "../../common/MemoryPool/MemoryPool.h"
#include "../../common/threads/PeriodicThread.h"

#include "../../common/DateTime/IClockDriver.h"

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
#include "../Commands/JetstreamCommands.h"

#include "../Commands/CommandList.h"

#include "../MessageParser.h"

#include "../../comms/CommStateMachine/CommsSignals.h"
#include "../../comms/CommStateMachine/BidirectionalCommsAvailableState.h"
#include "../../comms/CommStateMachine/UnidirectionalCommsAvailableState.h"
#include "../../comms/CommStateMachine/CommsUnavailableState.h"

#include "../../comms/CommQueue.h"

// TODO: this needs to be replaced with debug print message bit-switches.
#undef JSEVENT_DEBUG_PRINT

namespace Terso
{
   jetstreamEvent::jetstreamEvent(std::string deviceAccessKey,
                                  std::string deviceSerialNumber,
                                  IClockDriver* clockDriver,
                                  eJetstreamMessage jsMessageID)
            : m_logicalDeviceID(deviceSerialNumber),
              m_eventTime(),
              m_clockDriver(clockDriver),
              m_commQueue(NULL),
              m_eventUrl(),
              m_jsMessage(jsMessageID),
              m_deviceAccessKey(deviceAccessKey)
   {
      // intentionally left blank.
   }

   jetstreamEvent::jetstreamEvent()
      : m_logicalDeviceID(),
        m_eventTime(),
        m_clockDriver(NULL),
        m_commQueue(NULL),
        m_eventUrl(),
        m_jsMessage(eNone)
   {
      // intentionally left blank
   }

   jetstreamEvent::~jetstreamEvent()
   {
      // intentionally left blank
   }

   void jetstreamEvent::setCommunicationQueue(CommQueue* commQueue)
   {
      m_commQueue = commQueue;

      ostringstream urlBuilder;
      urlBuilder << "https://"
                 << m_commQueue->getJetstreamURL()
                 << "/v1.0/device/?AccessKey="
                 << m_deviceAccessKey;

      m_eventUrl = urlBuilder.str();
   }

   void jetstreamEvent::PostToJetstream()
   {
      #ifdef JSEVENT_DEBUG_PRINT
         // TODO: modify this to utilize Debug Print masks and log to the
         //       ILogger or ILoggable
         cout << "Queuing the Jetstream Event" << endl;
      #endif

      // get the event xml from the child's specific Event's implementation of
      // the BuildXML() function.
      if (NULL != m_commQueue)
      {
         string event_xml = buildXML();
         CommRecord record(POST, m_eventUrl, event_xml, m_jsMessage, m_clockDriver->getCurrentTime());
         m_commQueue->enqueue(record);
      }
   }
}
