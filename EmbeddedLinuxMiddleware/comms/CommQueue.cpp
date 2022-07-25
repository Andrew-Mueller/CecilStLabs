#include <sys/socket.h>
#include <resolv.h>
#include <netdb.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#include "../common/tinyxml/tinyxml2.h"
using namespace tinyxml2;

#include "../common/basicTypes.h"
#include "../common/util/EnumByName.h"
#include "../common/DateTime/DateTime.h"
#include "../common/util/BasicUtil.h"
#include "../common/guid.h"

#include "../common/logging/LogEntry.h"
#include "../common/logging/ILogDriver.h"
#include "../common/logging/emptyLogDriver.h"
#include "../common/logging/ILoggable.h"

#include "../common/LinkedList/SingleLink.h"
#include "../common/LinkedList/SingleLinkedList.h"

#include "../common/DateTime/IClockDriver.h"

#include "../common/RingBuffer/RingBuffer.h"
#include "../common/MemoryPool/MemoryPool.h"

#include "../common/threads/PeriodicThread.h"

#include "RXData/RXData.h"
#include "NetUtil.h"
#include "SocketError.h"
#include "SocketBase.h"
#include "Socket.h"
#include "SecureSession.h"
#include "SecureSocket.h"
#include "WebSocket.h"

#include "ICommProtocolResponse.h"
#include "ICommProtocol.h"

#include "HTTP/HTTPResponse.h"
#include "HTTP/HTTPS.h"

#include "WebSocketProtocol/WebSocketResponse.h"
#include "WebSocketProtocol/WebSocketProtocol.h"

#include "../common/stateMachine/IState.h"
#include "../common/stateMachine/Transition.h"
#include "../common/stateMachine/ITransitionHandler.h"
#include "../common/stateMachine/State.h"
#include "../common/stateMachine/StateMachine.h"

#include "CommRecord.h"
#include "CommDAL.h"

#include "../common/util/NameValuePair.h"
#include "../common/config/IConfig.h"
#include "../common/InternalEvent/IInternalEvent.h"
#include "../common/InternalEvent/IInternalEventHandler.h"
#include "../common/InternalEvent/InternalEventRegistry.h"

#include "CommStateMachine/CommsSignals.h"

#include "CommStateMachine/BidirectionalCommsAvailableState.h"
#include "CommStateMachine/UnidirectionalCommsAvailableState.h"
#include "CommStateMachine/CommsUnavailableState.h"

#include "CommQueue.h"

namespace CecilStLabs
{
   CommQueue::CommQueue(std::string queueFileName,
                        std::string endpointURL,
                        messageParser& parser,
                        ICommProtocol* https,
                        ICommProtocol* websocketprot,
                        uint8_t commsReconnectInterval,
                        ILogDriver& logDriver)
      : m_commDAL(queueFileName),
        m_httpsProtocol(https),
        m_websocketProtocol(websocketprot),
        m_commStateMachine(),

        m_biCommsAvailableState(&m_commDAL, m_websocketProtocol, parser),
        m_uniCommsAvailableState(&m_commDAL, m_websocketProtocol, m_httpsProtocol, parser, false),
        m_uniSingleCommsAvailableState(&m_commDAL, m_websocketProtocol, m_httpsProtocol, parser, true),
        m_commsUnavailableState(&m_commDAL, m_websocketProtocol, m_httpsProtocol, parser, commsReconnectInterval),

        m_transHTTP200(SIGNAL_HTTP_200, &m_biCommsAvailableState),
        m_transHTTP_NON_HTTP200(SIGNAL_NON_HTTP_200, &m_commsUnavailableState),
        m_transWS_CLOSED(SIGNAL_WEBSOCKET_CLOSED, &m_uniCommsAvailableState),
        m_transUnavailableWS_OPEN(SIGNAL_WEBSOCKET_OPEN, &m_biCommsAvailableState),
        m_transUnidirectionalWS_OPEN(SIGNAL_WEBSOCKET_OPEN, &m_biCommsAvailableState),
        m_transWS_Single(SIGNAL_ONE_HTTP, &m_uniSingleCommsAvailableState),
        m_transitionSingle_NON(SIGNAL_NON_HTTP_200, &m_commsUnavailableState),
        m_transitionSingle_WS(SIGNAL_WEBSOCKET_OPEN, &m_biCommsAvailableState)
   {
      m_commDAL.setLogDriver(&logDriver);

      m_httpsProtocol->setLogDriver(&logDriver);
      m_httpsProtocol->setSecureSession(m_session);

      m_websocketProtocol->setLogDriver(&logDriver);

      m_biCommsAvailableState.setLogDriver(&logDriver);
      m_biCommsAvailableState.addTransition(&m_transWS_CLOSED);
      m_biCommsAvailableState.addTransition(&m_transWS_Single);

      m_uniCommsAvailableState.setLogDriver(&logDriver);
      m_uniCommsAvailableState.addTransition(&m_transHTTP_NON_HTTP200);
      m_uniCommsAvailableState.addTransition(&m_transUnidirectionalWS_OPEN);

      m_uniSingleCommsAvailableState.setLogDriver(&logDriver);
      m_uniSingleCommsAvailableState.addTransition(&m_transitionSingle_NON);
      m_uniSingleCommsAvailableState.addTransition(&m_transitionSingle_WS);

      m_commsUnavailableState.setLogDriver(&logDriver);
      m_commsUnavailableState.addTransition(&m_transHTTP200);
      m_commsUnavailableState.addTransition(&m_transUnavailableWS_OPEN);

      m_biCommsAvailableState.setTransitionHandler(&m_commStateMachine);
      m_uniCommsAvailableState.setTransitionHandler(&m_commStateMachine);
      m_uniSingleCommsAvailableState.setTransitionHandler(&m_commStateMachine);
      m_commsUnavailableState.setTransitionHandler(&m_commStateMachine);

      // NOTE: add the unavailable state first to ensure that it is the first
      //       state that the state machine enters.
      m_commStateMachine.add(&m_biCommsAvailableState);
      m_commStateMachine.add(&m_uniCommsAvailableState);
      m_commStateMachine.add(&m_uniSingleCommsAvailableState);
      m_commStateMachine.add(&m_commsUnavailableState);
   }

   CommQueue::~CommQueue()
   {
      // intentionally left blank.
   }

   void CommQueue::enqueue(CommRecord& newRecord)
   {
      m_commDAL.insert(newRecord);
   }

   CommRecord* CommQueue::get()
   {
      return m_commDAL.getOldestRecord();
   }

   void CommQueue::remove(CommRecord& record)
   {
      m_commDAL.remove(record.getID());
   }

   void CommQueue::execute()
   {
      m_commStateMachine.execute();
   }
}
