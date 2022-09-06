#include <sys/socket.h>
#include <resolv.h>
#include <netdb.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#include <pthread.h>                            //lint !e537

#include <iostream>
#include <sstream>

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

#include "CommandList.h"
#include "MessageParser.h"
#include "CommsSignals.h"
#include "BidirectionalCommsAvailableState.h"

namespace CecilStLabs
{

   BidirectionalCommsAvailableState::BidirectionalCommsAvailableState(CommDAL* commDAL,
                                                                      ICommProtocol* protocol,
                                                                      MessageParser& parser)
     : m_commDAL(commDAL),
       m_protocol(protocol),
       m_parser(parser)
   {
      // intentionally left blank.
   }

   BidirectionalCommsAvailableState::~BidirectionalCommsAvailableState()
   {
      // intentionally left blank.
   }

   void BidirectionalCommsAvailableState::enter()
   {
      // logic to perform when the state is entered
      // (currently none).
      getLogDriver()->log("Entering BidirectionalCommsAvailableState", LoggingDebug);
   }

   void BidirectionalCommsAvailableState::execute()
   {
      std::stringstream debugMsg;
      WebSocketResponse* resp;

      if (NULL != m_protocol && m_protocol->IsActive())
      {
         //
         // TODO:: first get all messages waiting to be read.
         // after each message is received though we need to return the memory to the pool (owned by the websocket).
         m_protocol->parseResponse(1, 0);
         resp = (WebSocketResponse*)m_protocol->getResponse();
         if(resp->getMessageBody().size() > 0)
         {
            // TODO: define the message id's in an enumerated type.
            //       for now just passing 0
            m_parser.parse(0, resp->getMessageBody());
         }

         if( !m_protocol->IsConnected() )
         {
            debugMsg.str("");
            debugMsg << "BidirectionalCommsAvailableState::execute - WebSocket disconnected - signal to switch to HTTPS";
            getLogDriver()->Log(debugMsg.str(), LoggingDebug);

            // send the signal that websocket communications are unavailable.
            signal(SIGNAL_WEBSOCKET_CLOSED);
         }
         else
         {
            CommRecord* record = NULL;

            // send the oldest message until queue is empty.
            // ask the queue for the next message until all messages are sent.
            record = m_commDAL->getOldestRecord();

            while (NULL != record )
            {
               // don't try to send a zero length message out websocket
               if( 0 == record->getMessage().size() && (POST == record->getMethod() || GET == record->getMethod()) )
               {
                  // go to HTTPS single state, this is some sort of POST or GET with no message body
                  debugMsg.str("");
                  debugMsg << "BidirectionalCommsAvailableState::execute - switch to HTTPS, this is a POST or GET with no message body" << std::endl;
                  getLogDriver()->log(debugMsg.str(), LoggingDebug);

                  signal(SIGNAL_ONE_HTTP);
                  break;
               }
               else
               {
                  if( false == sendWebsocketRecord( record ) )
                  {
                     // some error in sending record, flip to HTTPS
                     signal(SIGNAL_WEBSOCKET_CLOSED);
                     break;
                  }

               }

               // get next message until all messages are sent.
               record = m_commDAL->getOldestRecord();
            }
         }
      }
      else
      {
         // flip out of this state, the protocol is not active
         signal(SIGNAL_WEBSOCKET_CLOSED);
      }


      // if connection fails or is closed, transition to unidirectional state
   }

   void BidirectionalCommsAvailableState::exit()
   {
      // logic to perform when exiting thes state.
      // Ensure the websocket is closed.

      // TODO: verify this does what I think it does.
      m_protocol->completeRequest();

      getLogDriver()->log("Exiting BidirectionalCommsAvailableState.", LoggingDebug);
   }

   bool BidirectionalCommsAvailableState::sendWebsocketRecord( CommRecord* record )
   {
      std::stringstream debugMsg;
      bool bSuccess = true;

      // attempt to send the record.
      debugMsg.str("");
      debugMsg << "BidirectionalCommsAvailableState::sendWebsocketRecord - WS Sending Record ID: " << (int)record->getMessageID();
      debugMsg << "  Method: " << (int)record->getMethod();
      debugMsg << "  URL: " << record->getURL();
      debugMsg << "  Msg: " << record->getMessage();
      getLogDriver()->log(debugMsg.str(), LoggingDebug);

      if( m_protocol->sendRequest( record->getMethod(),
                                   record->getURL(),
                                   record->getMessage()) )
      {
         m_protocol->parseResponse(1, 10);
         WebSocketResponse* resp = (WebSocketResponse*)m_protocol->getResponse();
         
         std::cout << "after parseResponse - " << resp->getStatusCode() << std::endl;

         if( WS_STATUS_OK == resp->getStatusCode() || WS_STATUS_ERROR == resp->getStatusCode() )
         {
            // remove the message even if it is error, so it doesn't just repeat forever
            //  the error is likely a bad message so, it won't magically change to a good message
            m_commDAL->remove(record->getID());

            if( WS_STATUS_ERROR == resp->getStatusCode() )
            {
               debugMsg.str("");
               debugMsg << "BidirectionalCommsAvailableState::sendWebsocketRecord - WebSocket error - signal to switch to HTTPS";
               getLogDriver()->Log(debugMsg.str(), LoggingDebug);

               // send the signal that websocket communications are unavailable.
              bSuccess = false;
            }
         }
         else if (WS_STATUS_TIMEOUT == resp->getStatusCode() )
         {
            // the WS_STATUS_TIMEOUT does NOT remove the message, because that indicates some sort
            //   of a comm error, and can come back

            debugMsg.str("");
            debugMsg << "BidirectionalCommsAvailableState::sendWebsocketRecord - WebSocket timeout - signal to switch to HTTPS";
            getLogDriver()->Log(debugMsg.str(), LoggingDebug);

            // send the signal that websocket communications are unavailable.
            bSuccess = false;
        }


      }
      else
      {
         // ok to log here because it will only fail in this state once then go to unavailable state
         // don't want to create an endless string of error messages
         getLogDriver()->log(std::string("BidirectionalCommsAvailableState::sendWebsocketRecord - error in sendRequest"), LoggingDebug);

         // send the signal that communications are unavailable.
         bSuccess = false;
      }


      return bSuccess;
   }
}
