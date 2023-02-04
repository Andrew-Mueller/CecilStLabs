#include <sys/socket.h>
#include <resolv.h>
#include <netdb.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#include <pthread.h>                            //lint !e537

#include <sstream>
#include <iostream>

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
#include "UnidirectionalCommsAvailableState.h"

#define COMMS_AVAIL_STATE_DEBUG_PRINT

namespace CecilStLabs
{

   UnidirectionalCommsAvailableState::UnidirectionalCommsAvailableState(CommDAL* commDAL,
                                            ICommProtocol* websocket,
                                            ICommProtocol* https,
                                            MessageParser& parser,
                                            bool single)
      : m_commDAL(commDAL),
        m_websocketProtocol(websocket),
        m_httpsProtocol(https),
        m_parser(parser),
        m_single(single)
   {
      // intentionally left blank.
   }

   UnidirectionalCommsAvailableState::~UnidirectionalCommsAvailableState()
   {
      // intentionally left blank.
   }

   void UnidirectionalCommsAvailableState::enter()
   {
      // logic to perform when the state is entered
      // (currently none).
      getLogDriver()->Log("Entering UnidirectionalCommsAvailableState", LoggingDebug);
   }

   void UnidirectionalCommsAvailableState::execute()
   {
      std::stringstream debugMsg;
      bool bExecSuccess = true;

      // send the oldest message until queue is empty.
      CommRecord* record;

      if (NULL != m_httpsProtocol)
      {
         // ask the queue for the next message until all messages are sent.
         record = m_commDAL->getOldestRecord();

         while (NULL != record)
         {
            std::cout << "UnidirectionalCommsAvailableState::execute - found record" << std::endl;

            // attempt to send the record.
            // NOTE: the Request() method blocks until a response is received, or
            // the network times out.
            debugMsg.str("");
            debugMsg << "UnidirectionalCommsAvailableState::execute - Sending Record ID: " << (int)record->getMessageID();
            debugMsg << "  Method: " << (int)record->getMethod();
            debugMsg << "  URL: " << record->getURL();
            debugMsg << "  Msg: " << record->getMessage();
            getLogDriver()->Log(debugMsg.str(), LoggingDebug);

            if (m_httpsProtocol->sendRequest(record->getMethod(),
                                             record->getURL(),
                                             record->getMessage()))
            {
               m_httpsProtocol->parseResponse(1);
               HTTPResponse* resp = (HTTPResponse*)m_httpsProtocol->getResponse();

               if (STATUS_OK == resp->getStatusCode())
               {
                  // parse the response to hand it back to the appropriate command.
                  m_parser.parse(record->getMessageID(), resp->getMessageBody());

                  // remove the message successfully sent from the queue.
                  m_commDAL->remove(record->getID());

                  debugMsg.str("");
                  debugMsg << "UnidirectionalCommsAvailableState::execute - Response Code: " << resp->getStatusCode() << " ";
                  debugMsg << "Response: " << resp->getMessageBody();
                  getLogDriver()->Log(debugMsg.str(), LoggingDebug);
               }
               else
               {
                  // the return code indicates the message could not be sent.
                  // log some info for debugging, and send a signal to switch states.

                  debugMsg.str("");
                  debugMsg << "FAILED sending message! - Response code: " << resp->getStatusCode();

                  // Log as an error. This message is then removed from queue
                  getLogDriver()->Log(debugMsg.str(), LoggingError);

                  // now log a debug level message with the response
                  debugMsg.str("");
                  debugMsg << "FAILED sending message! - Response code: " << resp->getStatusCode();
                  debugMsg << "  Response: " << resp->getMessageBody();
                  getLogDriver()->Log(debugMsg.str(), LoggingDebug);

                  // remove the message from the queue so it doesn't get stuck in an endless send/fail loop
                  // it is already sent to the syslog
                  m_commDAL->remove(record->getID());

                   // send the signal that communications are unavailable.
                  signal(SIGNAL_NON_HTTP_200);

                  // close socket
                  m_httpsProtocol->completeRequest();

                  bExecSuccess = false;

                  //don't pull any more messages
                  break;
               }
            }
            else
            {
               // ok to log here because it will only fail in this state once then go to unavailable state
               // don't want to create an endless string of error messages
               getLogDriver()->Log(std::string("UnidirectionalCommsAvailableState::execute - error in sendRequest"), LoggingDebug);

               // send the signal that communications are unavailable.
               signal(SIGNAL_NON_HTTP_200);

               bExecSuccess = false;

               // break out because there may be a connection problem
               break;
            }

            // close socket
            m_httpsProtocol->completeRequest();

            if( true == m_single )
            {
               // drop out of loop
               record = NULL;
            }
            else
            {
               // get next message until all messages are sent.
               record = m_commDAL->getOldestRecord();
            }
         }
      }

      if(bExecSuccess)
      {
         if( NULL != m_websocketProtocol && m_websocketProtocol->IsActive() )
         {
            // try to go back and do websocket if it is active
            signal(SIGNAL_WEBSOCKET_OPEN);
         }
      }
   }

   void UnidirectionalCommsAvailableState::exit()
   {
      // logic to perform when exiting thes state.
      // (currently none).

      getLogDriver()->Log("Exiting UnidirectionalCommsAvailableState.", LoggingDebug);
   }
}
