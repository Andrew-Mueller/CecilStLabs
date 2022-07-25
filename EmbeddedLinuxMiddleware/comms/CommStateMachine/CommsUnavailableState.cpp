#include <sys/socket.h>
#include <resolv.h>
#include <netdb.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#include <pthread.h>                            //lint !e537

#include <iostream>
using namespace std;

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

#include "../RXData/RXData.h"
#include "../SocketError.h"
#include "../SocketBase.h"
#include "../Socket.h"
#include "../SecureSession.h"
#include "../SecureSocket.h"

#include "../ICommProtocolResponse.h"
#include "../ICommProtocol.h"
#include "../HTTP/HTTPResponse.h"
#include "../HTTP/HTTPS.h"
#include "../WebSocket.h"
#include "../WebSocketProtocol/WebSocketResponse.h"
#include "../WebSocketProtocol/WebSocketProtocol.h"

#include "../CommRecord.h"
#include "../CommDAL.h"

#include "../../common/tinyxml/tinyxml2.h"
using namespace tinyxml2;

#include "../../common/config/IConfig.h"
#include "../../common/config/NullConfig.h"

#include "../../common/util/NameValuePair.h"
#include "../../common/InternalEvent/IInternalEvent.h"
#include "../../common/InternalEvent/IInternalEventHandler.h"
#include "../../common/InternalEvent/InternalEventRegistry.h"

#include "../../common/stateMachine/IState.h"
#include "../../common/stateMachine/Transition.h"
#include "../../common/stateMachine/ITransitionHandler.h"
#include "../../common/stateMachine/State.h"

#include "CommsSignals.h"
#include "CommsUnavailableState.h"

namespace CecilStLabs
{
   CommsUnavailableState::CommsUnavailableState(CommDAL* commDAL,
                                                ICommProtocol* websocket,
                                                ICommProtocol* https,
                                                messageParser& parser,
                                                uint8_t reconnectInterval
                                               )
      : m_commDAL(commDAL),
        m_websocketProtocol(websocket),
        m_httpsProtocol(https),
        m_parser(parser),
        m_reconnectInterval(reconnectInterval),
        m_reconnectCounter(0)
   {
      // intentionally left blank.
   }

   CommsUnavailableState::~CommsUnavailableState()
   {
      // intentionally left blank.
   }

   void CommsUnavailableState::enter()
   {
      // logic to perform when the state is entered
      // (currently none).
      getLogDriver()->log("Entering CommsUnavailableState", LoggingDebug);

      m_reconnectCounter = 0;
   }

   void CommsUnavailableState::exit()
   {
      getLogDriver()->log("Exiting CommsUnavailableState", LoggingDebug);
   }

   void CommsUnavailableState::execute()
   {
      stringstream debugMsg;
      bool tryHTTPS = true;

      // only try to reconnect if the interval has expired
      if( m_reconnectCounter++ >= m_reconnectInterval )
      {
         debugMsg.str("");
         debugMsg << "CommsUnavailableState::execute - reconnect counter done, try to connect again";
         getLogDriver()->Log(debugMsg.str(), LoggingDebug);

         // reset counter in case it stays in this state
         m_reconnectCounter = 0;

         if (NULL != m_websocketProtocol && m_websocketProtocol->IsActive())
         {
            m_websocketProtocol->parseResponse(1);
            WebSocketResponse* resp = (WebSocketResponse*)m_websocketProtocol->getResponse();

            if(resp->getMessageBody().size() > 0)
            {
               m_parser.parse(ePollForCommands_Msg, resp->getMessageBody());
            }

            if( m_websocketProtocol->IsConnected() && WS_STATUS_OK == resp->getStatusCode() )
            {
               debugMsg.str("");
               debugMsg << "CommsUnavailableState::execute - WebSocket connected - signal to switch to Websocket";
               getLogDriver()->Log(debugMsg.str(), LoggingDebug);

               // send the signal that websocket communications are available.
               signal(SIGNAL_WEBSOCKET_OPEN);
               tryHTTPS = false;
            }
         }

         if( tryHTTPS )
         {

            // ask the queue for the next message until all messages are sent.
            CommRecord* record = m_commDAL->getOldestRecord();

            if ((NULL != record) &&
               (NULL != m_httpsProtocol))
            {
               // attempt to send the record
               debugMsg.str("");
               debugMsg << "CommsUnavailableState::execute - Sending HTTPS Record ID: " << (int)record->getMessageID();
               debugMsg << "  Method: " << (int)record->getMethod();
               debugMsg << "  URL: " << record->getURL();
               debugMsg << "  Msg: " << record->getMessage();
               getLogDriver()->log(debugMsg.str(), LoggingDebug);

               if (m_httpsProtocol->sendRequest(record->getMethod(),
                                          record->getURL(),
                                          record->getMessage()))
               {

                  m_httpsProtocol->parseResponse(1);

                  HTTPResponse* resp = (HTTPResponse*)m_httpsProtocol->getResponse();

                  if (STATUS_OK == resp->getStatusCode())
                  {
                     // parse the response to hand it back to the appropriate command.
                     m_parser.parse(record->getMessageID(),
                                    resp->getMessageBody());

                     m_commDAL->remove(record->getID());

                     debugMsg.str("");
                     debugMsg << "CommsUnavailableState::execute - HTTPS Response Code: " << resp->getStatusCode();
                     debugMsg << "  Response: " << resp->getMessageBody();
                     getLogDriver()->log(debugMsg.str(), LoggingDebug);

                     // send the signal that communications are available.
                     signal(SIGNAL_HTTP_200);
                  }
                  else
                  {
                     // error sending the message

                     debugMsg.str("");
                     debugMsg << "FAILED sending message! - Response code: " << resp->getStatusCode();

                     // Log as an error. This message is then removed from queue
                     getLogDriver()->log(debugMsg.str(), LoggingError);

                     // now log a debug level message with the response
                     debugMsg.str("");
                     debugMsg << "FAILED sending message! - Response code: " << resp->getStatusCode();
                     debugMsg << "  Request: " << record->getMessage();
                     debugMsg << "  Response: " << resp->getMessageBody();
                     getLogDriver()->log(debugMsg.str(), LoggingDebug);

                     // remove the message from the queue so it doesn't get stuck in an endless send/fail loop
                     // it is already sent to the syslog
                     m_commDAL->remove(record->getID());


                     // NOTE: DO NOT queue an error that the message could not be sent,
                     //       this leads to a queue full of "communications unavailable" messages.
                  }

                  // this makes sure the socket gets closed
                  m_httpsProtocol->completeRequest();


               }
               else
               {
                  // Log this as debug level. the level won't be on debug unless someones is actually debugging an issue
                  // Otherwise this could lead to log getting filled with errors
                  getLogDriver()->log(string("CommsUnavailableState::execute - error in sendRequest"), LoggingDebug);
               }

            }
            else
            {
               if (NULL == record)
               {
                  getLogDriver()->log(string("CommsUnavailableState::execute - record is null"), LoggingDebug);
               }

               if (NULL == m_httpsProtocol)
               {
                  getLogDriver()->log(string("CommsUnavailableState::execute - protocol is empty."), LoggingDebug);
               }
            }
         }
      }
    }
}
