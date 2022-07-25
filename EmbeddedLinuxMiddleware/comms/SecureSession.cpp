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
#include "../common/CriticalSection/CriticalSection.h"

#include "SecureSession.h"

namespace CecilStLabs
{

   SecureSession::SecureSession()
      : m_ctx(0),
        m_ssl(0)
   {
      // start up the OpenSSL Library.
      SSL_library_init();

      // Load cryptos, et.al.
      OpenSSL_add_all_algorithms();

      // Bring in and register error messages
      SSL_load_error_strings();

      // perform the SSL handshaking.
      SSL_METHOD *method;
      method = (SSL_METHOD*)TLSv1_client_method();

      m_ctx = SSL_CTX_new(method);

      if (NULL == m_ctx)
      {
         ERR_print_errors_fp(stderr);
      }

      // setup the new SSL connection state
      m_ssl = SSL_new(m_ctx);

      if (NULL == m_ssl)
      {
         ERR_print_errors_fp(stderr);
      }
   }

   SecureSession::~SecureSession()
   {
      // release the connection state
      SSL_free(m_ssl);

      // release the SSL Context
      SSL_CTX_free(m_ctx);
   }

   bool SecureSession::AttachSocket(socket_t* socket)
   {
      bool isAttached = false;

      // NOTE: it is expected that the base BSD connect() method occurs before
      //       this session is connect.

      if ((NULL != socket) &&
          (NULL != m_ssl))
      {
         // attach the socket to the ssl descriptor
         SSL_set_fd(m_ssl, *socket);

         // negotiate the SSL TLS
         if (-1 == SSL_connect(m_ssl))
         {
            ERR_print_errors_fp(stderr);
         }
         else
         {
            isAttached = true;
         }
      }

      return isAttached;
   }

   bool SecureSession::ShutdownSocket(socket_t* socket)
   {
      bool isShutdown = false;

      if ((NULL != socket) &&
          (NULL != m_ssl))
      {
         if (-1 == SSL_shutdown(m_ssl))
         {
            ERR_print_errors_fp(stderr);
         }
         else
         {
            isShutdown = true;
         }
      }

      return isShutdown;
   }

   SSL* SecureSession::GetConnectionState()
   {
      return m_ssl;
   }

   void SecureSession::Lock()
   {
      m_critSection.Enter();
   }

   void SecureSession::Unlock()
   {
      m_critSection.Exit();
   }
}
