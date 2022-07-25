#ifndef SECURESOCKET_H_
#define SECURESOCKET_H_

namespace CecilStLabs
{
   class SecureSocket : public SocketBase
   {
      public:

         /**
          * Create an empty Secure Socket object.
          */
         SecureSocket();

         /**
          * release the resources consumed by the Secure Socket object.
          */
         virtual ~SecureSocket();

         /**
          * Negotiate the connection using TLS for the SecureSocket connection.
          *
          * @param destination_ip The IP address of the server to connect to.
          */
         virtual SocketError Connect(std::string destination_ip);

         /**
          * Cleanup and close the TLS required for the SecureSocket connection.
          */
         virtual void Close();

         /**
          * Setter for the secure session (OpenSSL manager) for this secure
          * socket.
          *
          * @param session Reference to the SSL SecureSession object to set.
          */
         void SetSecureSession(SecureSession& session);

      protected:

         /**
          * The default Secure Socket Layer port.
          */
         static const uint16_t SSL_PORT = 443;

         /**
          * The low level write to the secure socket implementation
          * Error codes can be found: www.openssl.org/docs/ssl/SSL_get_error.html
          *
          * @param buffer The byte buffer to write to the secure socket.
          * @param numBytes The number of bytes that make up the buffer to write.
          */
         int16_t SockWrite(uint8_t* buffer, uint16_t numBytes);

         /**
          * The low level read from the secure socket implementation.
          *
          * @param buffer The byte buffer to read data into from the socket.
          * @param numBytes The number of bytes to read into the buffer.
          */
         int16_t SockRead(uint8_t* buffer, uint16_t numBytes);

      private:

         /**
          * The Session used for managing the shared Secure Session between sockets.
          */
         SecureSession* m_secureSession;
   };
}
#endif
