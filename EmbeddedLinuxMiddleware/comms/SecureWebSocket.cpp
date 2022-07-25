#include <iostream>
#include <sstream>
using namespace std;

#include <pthread.h>

#include <sys/socket.h>
#include <resolv.h>
#include <netdb.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#include "../common/basicTypes.h"
#include "../common/util/EnumByName.h"
#include "../common/DateTime/DateTime.h"
#include "../common/util/BasicUtil.h"

#include "../common/logging/LogEntry.h"
#include "../common/logging/ILogDriver.h"
#include "../common/logging/emptyLogDriver.h"
#include "../common/logging/ILoggable.h"

#include "../common/RingBuffer/RingBuffer.h"
#include "../common/MemoryPool/MemoryPool.h"
#include "../common/threads/PeriodicThread.h"

#include "RXData/RXData.h"

#include "SocketError.h"
#include "NetUtil.h"

#include "SocketBase.h"
#include "Socket.h"
#include "SecureSession.h"
#include "SecureSocket.h"
#include "SecureWebSocket.h"

namespace CecilStLabs
{
   const std::string SecureWebSocket::CREATE_KEY = "dGhlIHNhbXBsZSBub25jZQ==";

   SecureWebSocket::SecureWebSocket()
      : m_application_path(""),
        m_application_host(""),
        m_key("dGhlIHNhbXBsZSBub25jZQ==")
   {

      // TODO: hard code the mask used for messages for now.
      //       in the future each frame could use random mask bytes.
      m_mask[0] = 0x01;
      m_mask[1] = 0x23;
      m_mask[2] = 0x45;
      m_mask[3] = 0x67;
   }

   SecureWebSocket::~SecureWebSocket()
   {
      // intentionally left blank.
   }

   SocketError SecureWebSocket::Connect(std::string destination_ip)
   {
      SecureSocket::Connect(destination_ip);

      // create the thread for asynchronously receiving Web Socket data.
      m_running = true;
      uint8_t status = pthread_create(&m_rxThread, NULL, &ReceiveThreadWorker, this);

      BasicUtil::Assert((0 == status), __FILE__, __LINE__);

      if (!SendHeader())
      {
         pthread_mutex_lock(&m_printSem);

         #ifdef DEBUG_PRINT_WEBSOCKET
            cout << "Error sending the handshake" << endl;
         #endif

         pthread_mutex_unlock(&m_printSem);

         return SocketError(SocketError::error_sending_header);
      }

      // connect should block until the connection is
      // made and the websocket is ready for action.
      if (!CheckHeaderResponse())
      {
         pthread_mutex_lock(&m_printSem);

         #ifdef DEBUG_PRINT_WEBSOCKET
            cout << "Error with the handshake response." << endl;
         #endif

         pthread_mutex_unlock(&m_printSem);

         return SocketError(SocketError::error_parsing_header_response);
      }

      return SocketError(SocketError::connection_success);;
   }

   bool SecureWebSocket::SendHeader()
   {
      // NOTE: The Amazon Windows Server 2012
      // NOTE: the spec suggests \r\n to separate the lines of the request.
      //       The speedway responds to \r\n in a sprintf with a core dump.
      //       I can't find any documentation of why this might occur.

      ostringstream headerStr;
      headerStr << "GET " << m_application_path << " HTTP/1.1" << std::endl;
      headerStr << "Host: " << m_application_host << std::endl;
      headerStr << "Upgrade: websocket" << endl;
      headerStr << "Connection: Upgrade" << endl;
      headerStr << "Sec-Websocket-Key: " << m_key << endl;
      headerStr << "Sec-WebSocket-Version: 13" << endl << endl;



      pthread_mutex_lock(&m_printSem);
      cout << endl << "..::Request::.. " << (int)headerStr.str().length() << " bytes" << endl;
      cout << headerStr.str() << endl;
      pthread_mutex_unlock(&m_printSem);

      uint32_t status = 0;

      // NOTE: i feel like i should mutex protect the use of the m_socket object
      //       however, there is a race condition that can occur if the receive thread
      //       gets the mutex, and then blocks on the recv function (inside the critical section).
      //
      //if (MUTEX_SUCCESS == pthread_mutex_lock(&m_socketSemaphore))
      {
         status = send(m_socket, headerStr.str().c_str(), (int)headerStr.str().length(), 0);

         pthread_mutex_lock(&m_printSem);
         cout << "send status: " << status << " bytes sent." << endl;
         pthread_mutex_unlock(&m_printSem);

         //pthread_mutex_unlock(&m_socketSemaphore);
      }

      if (status != (uint32_t)headerStr.str().length())
      {
         pthread_mutex_lock(&m_printSem);
         cout << "sent less bytes than expected. sent: " << status <<
                 " expected: " << (int)headerStr.str().length() << endl;
         pthread_mutex_unlock(&m_printSem);
      }

      // TODO: is sending less bytes than expected an error?  Or do i manually need to handle sending the
      //       rest of the message?

      return (status == (uint32_t)headerStr.str().length());
   }

   bool SecureWebSocket::CheckHeaderResponse()
   {
      rxData_t* rxData = Read(10);

      if (NULL != rxData)
      {
         pthread_mutex_lock(&m_printSem);
         cout << "..::Response " << rxData->numBytes << " ::.." << endl;
         cout << rxData->data << endl;
         pthread_mutex_unlock(&m_printSem);

         // TODO: compare the response fields returned from the server against
         //       the values expected.

         // TODO: Sec-WebSocket-Key is a random bytes.  The spec says it then takes
         //       these bytes, concatinates a hard coded GUID string: 258EAFA5-E914-47DA-95CA-C5AB0DC85B11
         //       Then, it creates the Secure Hash Algorithm SHA1 hash from it
         //       and performs Base64 encoding. The resulting string of bytes
         //       needs to be used by both the server and the client, and this
         //       string won�t be used by network endpoints that do not understand
         //       the WebSocket protocol. Then, this value is copied in the
         //       Sec-WebSocket-Accept header field.

         // we are done with the data received so we need to return it back to the
         // memory pool.
         m_rxFramePool.free(rxData);
      }
      else
      {
         pthread_mutex_lock(&m_printSem);
         cout << "header no response" << endl;
         pthread_mutex_unlock(&m_printSem);
      }

      return true;
   }
}
