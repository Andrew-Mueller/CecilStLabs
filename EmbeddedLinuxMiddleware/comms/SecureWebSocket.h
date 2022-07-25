#ifndef SECUREWEBSOCKET_H_
#define SECUREWEBSOCKET_H_

namespace CecilStLabs
{

   /**
    * Web Socket that communicates bi-directionally over TLS between the device
    * and the server.
    */
   class SecureWebSocket : public SecureSocket, public PeriodicThread, public ILoggable
   {
      public:

         /**
          * Create the TLS, SSL Secure websocket
          */
         SecureWebSocket();

         /**
          * Release all resources created and consumed by this
          * TLS, SSL Secure websocket
          */
         virtual ~SecureWebSocket();

         /**
          * Connect the Secure Web Socket.
          *
          * @param destination_ip The destination ip address of the
          *
          * @return Any error that occurs
          */
         SocketError Connect(std::string destination_ip);

         /**
          * Close the currently open Secure Web Socket connection.
          */
         void Close();


         /*bool Send(std::string message);

         RXData* Read(const uint8_t timeoutSeconds);
         */

      protected:

         /**
          * WebSocket Spec indicates that 126 in the payload length field indicates the next 2 bytes
          * should be interpreted as the payload length (uint16).
          */
         static const uint8_t EXT_PAYLOAD_BOUNDARY          = 126;

         /**
          * Websocket Spec indicates that 127  in the payload length field indicates the next 8 bytes
          * should be interpreted as the payload length.
          * the MSB is always 0.
          */
         static const uint8_t EXT_PAYLOAD_BOUNDARY_CONT     = 127;

         /**
          * The last packet must have the most significant bit set
          * to signal that this is the last packet of the message.
          */
         static const uint8_t FIN_BIT              = 0x80;

         /**
          * If the payload is masked, the mask bit must be set.
          */
         static const uint8_t MASK_BIT             = 0x80;

         /**
          * The bytes of the opcode are in the lower nibble of the first byte.
          */
         static const uint8_t OPCODE_MASK          = 0x0F;

         /**
          * Size of the received frame memory pool.
          */
         static const uint8_t NUM_FRAMES_IN_POOL = 10;

         /**
          * Size of the ring buffer used to store the address of
          * chunks in the memory pool.
          */
         static const uint8_t NUM_ADDYS_IN_RING = 10;

         /**
          * Key required in the websocket header.
          * This is the default key used during testing.
          */
         static const std::string CREATE_KEY;

         /**
          * Path to the websocket application
          */
         std::string m_application_path;

         /**
          * Path to the websocket host
          */
         std::string m_application_host;

         /**
          * Key used to initialize the websocket connection.
          * TODO: Decide if this should be auto-generated.
          *       It also might be worth making a constant.
          */
         std::string m_key;

         /**
          * Read data from the socket in a separate thread.
          */
         pthread_t m_rxThread;

         // NOTE: Keep m_mask as the last variable
         //       defined to ensure the order of the initialized variables.
         //       This is a cruddy side affect to c++ initialization lists
         //       (must be initialized in the order defined).
         /**
          * mask for sending data to the websocket server.
          * Websocket Spec requires all data sent from the client to the server
          * to be masked.
          */
         char m_mask[4];

         /**
          * The low level write to the socket implementation.
          *
          * @param buffer Buffer of bytes written to the socket
          * @param numBytes The number of bytes in the buffer.
          *
          * @return Number of bytes written to the socket.
          */
         virtual size_t SockWrite(uint8_t* buffer, uint16_t numBytes) = 0;

         /**
          * The low level read from the socket implementation.
          *
          * @param buffer Buffer of bytes to put the read bytes into.
          * @param numBytes The number of bytes received from the socket and
          *                 placed into the buffer.
          *
          * @return Number of bytes written to the socket.
          */
         virtual size_t SockRead(uint8_t* buffer, uint16_t numBytes) = 0;

         /**
          * Send the websocket header information to perform the upgrade to the
          * websocket connection.
          *
          * @return
          */
         bool SendHeader();

         /**
          * Checks the response from the websocket header upgrade.
          *
          * @return True if the header response is correct, False if the
          *         header response did not match what was expected.
          */
         bool CheckHeaderResponse();
   };

}
#endif
