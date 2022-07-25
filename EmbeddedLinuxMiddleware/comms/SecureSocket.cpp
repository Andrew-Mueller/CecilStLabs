#include <cstdio>
#include <errno.h>
#include <unistd.h>
#include <string.h>

#include <iostream>
#include <sstream>
using namespace std;

#include <sys/socket.h>
#include <resolv.h>
#include <netdb.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#include "../common/basicTypes.h"
#include "../common/util/BasicUtil.h"
#include "../common/RingBuffer/RingBuffer.h"
#include "../common/MemoryPool/MemoryPool.h"

#include "RXData/RXData.h"
#include "SocketError.h"
#include "SecureSession.h"
#include "SocketBase.h"
#include "Socket.h"
#include "SecureSocket.h"

namespace CecilStLabs
{

   SecureSocket::SecureSocket()
      : SocketBase(),
        m_secureSession()
   {
      // override the standard socket's port
      m_port = SSL_PORT;
   }

   SecureSocket::~SecureSocket()
   {
      // intentionally left blank
   }

   SocketError SecureSocket::Connect(string destination_ip)
   {
      // setup the basic socket connection to the server.
      SocketError status = SocketBase::Connect(destination_ip);

      if ((NULL != m_secureSession) &&
          (SocketError::connection_success == status.getError()))
      {
         m_secureSession->AttachSocket(&m_socket);
      }

      return status;
   }

   void SecureSocket::Close()
   {
      #ifdef DEBUG_PRINT
         cout << "Closing the Secure Socket." << endl;
      #endif

      if (NULL != m_secureSession)
      {
         m_secureSession->ShutdownSocket(&m_socket);
      }

      // perform the socket close action
      SocketBase::Close();
   }

   void SecureSocket::SetSecureSession(SecureSession& session)
   {
      m_secureSession = &session;
   }

   int16_t SecureSocket::SockWrite(uint8_t* buffer, uint16_t numBytes)
   {
      int16_t bytesSent = 0;

      if ((NULL != buffer) &&
          (0 < numBytes))
      {
         m_secureSession->Lock();

         SSL* connState = m_secureSession->GetConnectionState();
         bytesSent = SSL_write(connState,
                               buffer,
                               numBytes);

         m_secureSession->Unlock();
      }

      return bytesSent;
   }

   int16_t SecureSocket::SockRead(uint8_t* buffer, uint16_t numBytes)
   {
      int lastError = 0;
      int bytesRead = 0;
      int totRead = 0;

      if ((NULL != buffer) &&
          (0 < numBytes))
      {
         m_secureSession->Lock();

         ERR_clear_error();
         SSL* connState = m_secureSession->GetConnectionState();

         bytesRead = SSL_read(connState,
                              buffer,
                              RXData::MAX_RX_BYTES);
         lastError = SSL_get_error(connState, bytesRead);
         totRead = bytesRead;

         // set timout for socket to as low as possible now. Then the read below won't get stuck
         SetRxTimeout(0, 1);

         // read until an error is returned, then its done
         while( bytesRead > 0 && SSL_ERROR_NONE == lastError )
         {
             connState = m_secureSession->GetConnectionState();
             bytesRead = SSL_read(connState,
                                  &buffer[totRead],
                                  RXData::MAX_RX_BYTES);

            lastError = SSL_get_error(connState, bytesRead);

            if(SSL_ERROR_NONE == lastError)
            {
               totRead += bytesRead;
            }
         }

         // put timeout back
         SetRxTimeout(RX_TIMEOUT_SECONDS);

         if (0 == totRead)
         {
            // NOTE: either an error occurred or the read timed out.

            lastError = SSL_get_error(connState,
                                      lastError);

            switch (lastError)
            {
               case SSL_ERROR_NONE :
               {
                  // no error occurred.
                  break;
               }
               case SSL_ERROR_ZERO_RETURN :
               {
                  cout << "The TLS/SSL connection has been closed" << endl;
                  // TODO: signal that we are now disconnected
                  break;
               }
               case SSL_ERROR_WANT_READ :
               {
                  // this is a problem that we want to know about.
                  cout << "SSL Error - The read operation did not complete.  Try again later?" << endl;
                  break;
               }
               case SSL_ERROR_WANT_CONNECT :
               {
                  cout << "SSL Error - Want Connect?" << endl;
                  break;
               }
               case SSL_ERROR_WANT_X509_LOOKUP :
               {
                  cout << "SSL ERROR - X509 Callback?" << endl;
                  break;
               }
               default:
               {
                  cout << "SSL Error Unknown: " << lastError << endl;
                  break;
               }
            }
         }

         m_secureSession->Unlock();
      }

      return totRead;
   }
}
