#include <iostream>
#include <sstream>
#include <string>
using namespace std;

#include <sys/socket.h>
#include <resolv.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#include "../../common/basicTypes.h"
#include "../../common/util/EnumByName.h"
#include "../../common/DateTime/DateTime.h"
#include "../../common/util/BasicUtil.h"

#include "../../common/logging/LogEntry.h"
#include "../../common/logging/ILogDriver.h"
#include "../../common/logging/emptyLogDriver.h"
#include "../../common/logging/ILoggable.h"

#include "../../common/threads/PeriodicThread.h"

#include "../../comms/NetUtil.h"
#include "../../common/RingBuffer/RingBuffer.h"
#include "../../common/MemoryPool/MemoryPool.h"

#include "../RXData/RXData.h"
#include "../SocketError.h"
#include "../SocketBase.h"
#include "../Socket.h"
#include "../SecureSession.h"
#include "../WebSocket.h"

#include "../ICommProtocolResponse.h"
#include "../ICommProtocol.h"

#include "WebSocketResponse.h"
#include "WebSocketProtocol.h"

namespace CecilStLabs
{

   WebSocketProtocol::WebSocketProtocol(std::string hostname_str,
                                        std::string applicationPath,
                                        std::string deviceAccessKey)
      : m_webSocket(hostname_str, applicationPath),
        m_applicationPath(applicationPath),
        m_hostnameUrl(hostname_str),
        m_ip_address_str("0.0.0.0"),
        m_deviceAccessKey(deviceAccessKey)
   {
      // intentionally left blank.
   }

   WebSocketProtocol::~WebSocketProtocol()
   {
      // intentionally left blank
   }

   bool WebSocketProtocol::sendRequest(uint32_t protocolMethod,
                                       std::string location,
                                       std::string message)
   {

      // NOTE: the location for the websocket is already established, so the
      //       "location" variable is not used for the request.

      bool requestStatus = false;

      m_webSocket.setLogDriver(getLogDriver());

      // lookup the IP address of the host, but we only want to do this
      // once since it takes a while
      if (m_ip_address_str.empty() ||
          (0 == m_ip_address_str.compare("0.0.0.0")))
      {
         // convert the hostname to the IP addy
         m_ip_address_str = CecilStLabs::NetUtil::HostnameToIP(m_hostnameUrl);
      }

      SocketError connectionResult(SocketError::connection_success);

      if (!m_webSocket.IsConnected())
      {
         getLogDriver()->Log(std::string("WebSocketProtocol::sendRequest - not connected trying to..."), LoggingDebug);
         connectionResult = m_webSocket.Connect(m_ip_address_str, m_deviceAccessKey);
      }

      if (SocketError::connection_success == connectionResult.getError())
      {
         // send the appropriate request.
         if (m_webSocket.Send(message))
         {
            getLogDriver()->Log("WebSocketProtocol::sendRequest - request sent", LoggingDebug);

            // mark the request as sent (although we don't know whether it was
            // successful or not, just that it left the sender).
            requestStatus = true;
         }
         else
         {
            // Send failed so clean up and close the connection.
            m_webSocket.Close();
         }
      }
      else
      {
         // If connect failed, the socket is already closed
         getLogDriver()->Log("WebSocketProtocol::sendRequest - socket connect error", LoggingDebug);
      }

      return requestStatus;
   }

   void WebSocketProtocol::parseResponse(uint8_t expectedNumPackets, uint8_t timeoutSeconds)
   {
      bool doneReading = false;
      uint16_t packetsRead = 0;
      RXData* rxData = NULL;
      stringstream str;

      SocketError connectionResult(SocketError::connection_success);

      m_webSocket.setLogDriver(getLogDriver());

      m_response.clear();

      // lookup the IP address of the host, but we only want to do this
      // once since it takes a while
      if (m_ip_address_str.empty() ||
          (0 == m_ip_address_str.compare("0.0.0.0")))
      {
         // convert the hostname to the IP addy
         m_ip_address_str = CecilStLabs::NetUtil::HostnameToIP(m_hostnameUrl);
      }

      // in the websocket case, we want to re-connect when we attempt to read
      // as well.
      if (!m_webSocket.IsConnected())
      {
         getLogDriver()->Log(std::string("WebSocketProtocol::parseResponse - not connected trying to..."), LoggingDebug);
         connectionResult = m_webSocket.Connect(m_ip_address_str, m_deviceAccessKey);
      }

      if (SocketError::connection_success == connectionResult.getError())
      {
         while (!doneReading)
         {
            rxData = m_webSocket.Read(timeoutSeconds);
            if (NULL != rxData)
            {
               // stuff the read message into the WebSocketResponse
               string respStr((const char*)(rxData->GetDataPtr()));

               m_response.parse(respStr);

               // we are done with the data received so return it to the
               // memory pool
               m_webSocket.FreeReadData(rxData);

               packetsRead++;

               if (packetsRead == expectedNumPackets)
               {
                  doneReading = true;
               }

               str.clear();
               str << "WebSocketProtocol::parseResponse - RESPONSE BODY - " << m_response.getMessageBody() << endl;
               getLogDriver()->Log(str.str(), LoggingDebug);
            }
            else
            {

               m_response.requestTimedOut();

               str.clear();
               str << "WebSocketProtocol::parseResponse - timed out" << endl;
               getLogDriver()->Log(str.str(), LoggingDebug);

               doneReading = true;
            }
         }
      }
   }

   bool WebSocketProtocol::IsConnected()
   {
      return m_webSocket.IsConnected();
   }

   bool WebSocketProtocol::IsActive()
   {
      if(0 == m_applicationPath.size())
         return false;
      else
         return true;
   }

   void WebSocketProtocol::completeRequest()
   {
      // don't do anything...

      // NOTE: keep websocket open.
   }

   std::string WebSocketProtocol::getHostname()
   {
      return m_hostnameUrl;
   }
}
