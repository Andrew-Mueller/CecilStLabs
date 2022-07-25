#ifndef SECURE_SESSION_H
#define SECURE_SESSION_H

namespace CecilStLabs
{
   /**
    * Wrapper for the secure socket session implemented by OpenSSL.
    */
   class SecureSession
   {
      public:
         SecureSession();
         virtual ~SecureSession();

         /**
          * Attach the specified socket to this Secure Session.
          *
          * @param socket The BSD socket to attach this secure session to.
          *
          * @return True if the socket was attached successfully, False if an
          *         error occurred.
          */
         bool AttachSocket(socket_t* socket);

         /**
          * Shutdown the specified socket to this Secure Session.
          *
          * @param socket The BSD socket to attach this secure session to.
          *
          * @return True if the socket was shutdown successfully, False if an
          *         error occurred.
          */
         bool ShutdownSocket(socket_t* socket);

         /**
          * retrieve the Connection State for the Secure Session.
          *
          * @return the Secure Socket Layer used for this secure session.
          */
         SSL* GetConnectionState();

         /**
          * Lock the Secure session for use.
          */
         void Lock();

         /**
          * Unlock the Secure session for use.
          */
         void Unlock();

      protected:
         /**
          * The Secure Context in which a socket can be bound to.
          */
         SSL_CTX* m_ctx;

         /**
          * SSL connection state.
          */
         SSL* m_ssl;

         /**
          * Critical Section to protect the Secure Session.
          */
         CriticalSection m_critSection;
   };
}

#endif
