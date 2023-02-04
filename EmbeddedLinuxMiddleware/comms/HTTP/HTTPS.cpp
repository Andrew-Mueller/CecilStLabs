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

#include "../../comms/NetUtil.h"
#include "../../common/RingBuffer/RingBuffer.h"
#include "../../common/MemoryPool/MemoryPool.h"

#include "../RXData/RXData.h"
#include "../SocketError.h"
#include "../SocketBase.h"
#include "../Socket.h"
#include "../SecureSession.h"
#include "../SecureSocket.h"

#include "../ICommProtocolResponse.h"
#include "../ICommProtocol.h"

#include "HTTPResponse.h"
#include "HTTPS.h"

// TODO: when logging is implemented in all of these methods.
#undef HTTPS_DEBUG_PRINT

namespace CecilStLabs
{

   HTTPS::HTTPS(string hostname_str)
      : m_hostname_str(hostname_str),
        m_ip_address_str("0.0.0.0")
   {
      // intentionally left blank.
   }

   HTTPS::~HTTPS()
   {
      // intentionally left blank.
   }

   bool HTTPS::sendRequest(uint32_t eMethod,
                           std::string path_str,
                           std::string body_str)
   {
      bool requestStatus = false;

      // clear out the response from any previous request
      m_response.clear();

      // lookup the IP address of the host, but we only want to do this
      // once since it takes a while
      if (m_ip_address_str.empty() ||
          (0 == m_ip_address_str.compare("0.0.0.0")))
      {
         // convert the hostname to the IP addy
         m_ip_address_str = CecilStLabs::NetUtil::HostnameToIP(m_hostname_str);
      }
      SocketError connectionResult = m_sec_socket.Connect(m_ip_address_str);

      if (SocketError::connection_success == connectionResult.getError())
      {
         string reqStr;

         // build the appropriate request string.
         if (GET == (HTTP_Method_e)eMethod)
         {
            reqStr = BuildGET(path_str);
         }
         else if (POST == (HTTP_Method_e)eMethod)
         {
            reqStr = BuildPOST(path_str, body_str);
         }
         else
         {
            reqStr = "";

            getLogDriver()->Log("HTTP Method not supported", LoggingDebug);
         }

         // send the appropriate request.
         if (m_sec_socket.Send(reqStr))
         {
             getLogDriver()->Log("HTTPS::sendRequest - request sent", LoggingDebug);

            // mark the request as sent (although we don't know whether it was
            // successful or not, just that it left the sender).
            requestStatus = true;
         }
         else
         {
            // Send failed so clean up and close the socket
            m_sec_socket.Close();
         }
      }
      else
      {
         // If connect failed, the socket is already closed
         getLogDriver()->Log("HTTPS::sendRequest - socket connect error", LoggingDebug);
      }


      return requestStatus;
   }

   void HTTPS::parseResponse(uint8_t expectedNumPackets, uint8_t timeoutSeconds)
   {
      bool doneReading = false;
      uint16_t packetsRead = 0;
      RXData* rxData = NULL;
      uint8_t timeout_sec = timeoutSeconds;

      if (m_sec_socket.IsConnected())
      {
         while (!doneReading)
         {
            // TODO: receiving a message is flawed here... how do I know when the
            //       message is done being sent?
            //       Does the server close its end of the socket?
            rxData = m_sec_socket.Read(timeout_sec);

            if (NULL != rxData)
            {
               // parse the HTTP header and body into a strongly typed response object.
               string respStr = string((const char*)rxData->GetDataPtr());
               m_response.parse(respStr);

               // we are done with the data received so we need to return it back to the
               // memory pool.
               m_sec_socket.FreeReadData(rxData);

               // after the first chunk of data is received, crank the timeout way down since we are expecting
               // more data right away.
               timeout_sec = 1;

               packetsRead++;

               if (expectedNumPackets == packetsRead)
               {
                  getLogDriver()->Log("read the number of expected packets.", LoggingDebug);

                  doneReading = true;
               }
            }
            else
            {
               // if we get some data, but not all of it... is it a timeout?
               if (packetsRead == 0)
               {

                  getLogDriver()->Log("no response", LoggingDebug);

                  m_response.requestTimedOut();
               }

               doneReading = true;
            }
         }
      }  
   }

   void HTTPS::completeRequest()
   {
      // close the socket if it is still open.
      m_sec_socket.Close();
   }

   string HTTPS::getHostname()
   {
      return m_hostname_str;
   }

   string HTTPS::BuildGET(string path_str)
   {
      // NOTE: the spec suggests \r\n to separate the lines of the request.
      //       The speedway responds to \r\n in a sprintf, stream, etc. with a
      //       core dump.
      //
      //       Oddly enough, putting the \r and \n on separate lines
      //       seems to solve the problem.
      //
      //       I can't find any documentation of why this might occur.
      //       This might be specific to the ARM version of gcc used by the
      //       speedway (monte vista linux).
      //
      //       In the mean time, the fast and easy fix is to simply put the
      //       characters on separate lines.

      ostringstream getStr;
      getStr << "GET " << path_str << " HTTP/1.1" << "\r";
      getStr << "\n";

      getStr << "Host: " << m_hostname_str << "\r";
      getStr << "\n";

      // The GET must end with TWO carriage return line feeds.
      getStr << "\r";
      getStr << "\n";

      getStr << "\r";
      getStr << "\n";

      return getStr.str();
   }

   string HTTPS::BuildPOST(string path_str, string data_str)
   {
      // NOTE: see the comment in BuildGET above for information on the
      //       carriage return line feed.

      ostringstream postStr;

      postStr << "POST " << path_str << " HTTP/1.1" << "\r";
      postStr << "\n";

      postStr << "Host: " << m_hostname_str << "\r";
      postStr << "\n";

      postStr << "Content-Type: application/xml; charset=utf-8\r";
      postStr << "\n";

      postStr << "Content-Length: " << data_str.length() <<"\r";
      postStr << "\n";

      // add a blank line between the header and the body.
      if (!data_str.empty())
      {
         postStr << "\r";
         postStr << "\n";

         postStr << data_str;
      }

      // The request must end with TWO carriage return line feeds.
      postStr << "\r";
      postStr << "\n";
      postStr << "\r";
      postStr << "\n";

      return postStr.str();
   }

   void HTTPS::setSecureSession(SecureSession& secureSession)
   {
      this->m_sec_socket.SetSecureSession(secureSession);
   }
}
