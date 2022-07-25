#ifndef WEBSOCKETFRAME_H_
#define WEBSOCKETFRAME_H_

namespace CecilStLabs
{
   // specify the size for the first byte in the data frame.
   // this is required because enumeration
   typedef byte_t opcode_t;

   /**
    * RFC 6455 - Page 37
    * Definition for Data Frame types.
    * NOTE: You need to be careful with the size of the enumeration
    *       when cross compiling between multiple platforms.
    */
   enum WebsocketOpcodes_e
   {
      websocket_frag_continuation  = 0x00,
      websocket_frag_last          = 0x80,

      websocket_utf8_text          = 0x01,
      websocket_binary             = 0x02,

      // data types 3-7 are reserved for future used that are yet to be defined.
      opcode_res_3                 = 0x03,
      opcode_res_4                 = 0x04,
      opcode_res_5                 = 0x05,
      opcode_res_6                 = 0x06,
      opcode_res_7                 = 0x07,

      websocket_close              = 0x08,
      websocket_ping               = 0x09,
      websocket_pong               = 0x0A,

      opcode_extent
   };

   /**
    * Container for the web socket frame data.
    */
   class WebSocketFrame
   {
      public:

         /**
          * Create the web socket frame.
          */
         WebSocketFrame();

         /**
          * release all resources required by the web socket frame.
          */
         virtual ~WebSocketFrame();

         // getter and setter for the message to put in the web socket frame.
         inline void setMessage(const std::string message) { m_applicationMessage = message; };
         inline void setMessage(const char* messageBuffer) { m_applicationMessage = std::string(messageBuffer); };
         inline std::string getMessage() { return m_applicationMessage; };

         // getter and setter for the raw message buffer
         inline void setMessageBuffer(const uint8_t* const buffer, const uint16_t numBytes) { memcpy(m_messageBuffer, buffer, numBytes);};
         inline uint8_t* getMessageBuffer() { return m_messageBuffer; };

         /**
          * Get the encoded or decoded message buffer.
          */
         inline uint8_t* getCodedMessage() { return m_messageBuffer; };

         /**
          * Returns the number of bytes that are in the coded message buffer
          * after an encode or decode operation.
          *
          * @return The number of bytes in the encoded or decoded message.
          */
         inline uint16_t getLengthOfCodedMessage() { return m_numberOfMesssageBytes; };

         /**
          * Encode the data into the byte stream required by the web socket
          * protocol.
          */
         void encode();

         /**
          * Decode the byte stream received from the protocol into the
          * application readable format.
          *
          * @return true if the message was decoded properly, false if there
          *         was an error decoding the message.
          */
         bool decode();

         /**
          * build the frame used to send the web socket closing handshake.
          *
          * @param buffer The output buffer of bytes to place the closing
          *        handshake message into.
          * @param sizeOfBuffer The number of bytes in the specified buffer.
          *
          * @return True if the close frame was built successfully, False if it
          *         was not.
          */
         bool buildCloseFrame(uint8_t* buffer, const uint8_t sizeOfBuffer);

         /**
          * build the frame used to send a web socket ping.
          *
          * @param buffer The output buffer of bytes to place the ping message
          *        into.
          * @param sizeOfBuffer The number of bytes in the specified buffer.
          *
          * @return True if the ping frame was built successfully, False if it
          *         was not.
          */
         bool buildPingFrame(uint8_t* buffer, const uint8_t sizeOfBuffer);

         /**
          * The number of bytes in the websocket closing handshake frame.
          */
         static const uint8_t NUM_BYTES_IN_CLOSE_FRAME = 7;

         /**
          * The number of bytes in the websocket ping frame.
          */
         static const uint8_t NUM_BYTES_IN_PING_FRAME = 7;

      protected:

         // the following three values are used to indicate how many bytes are
         // used to define the extended payload length.
         static const uint8_t SINGLE_BYTE_LENGTH_LIMIT   = 125;
         static const uint8_t TWO_BYTE_LENGTH_LIMIT      = 126;
         static const uint8_t EIGHT_BYTE_LENGTH_LIMIT    = 127;

         /**
          * WebSocket Spec indicates that 126 in the payload length field indicates the next 2 bytes
          * should be interpretted as the payload length (uint16).
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
          * The maximum number of bytes in the encoded or decoded message.
          */
         static const int16_t MAX_BUFFER_SIZE     = MAX_INT16_VALUE;

         /**
          * Defines the interpretation of the 'payload data'.
          * Only uses the lower Four bits and is expected to be a member of
          * WebsocketOpcodes_e
          */
         opcode_t m_opcode;

         /**
          * Length of the payload data in bytes.
          * 0 - 125 ihat is the payload load length in this single byte.
          * 126 or 127 then the following extended length fields below are used.
          *
          * NOTE: The most significant bit is the Mask bit (indicates if the
          *       payload is masked or not).
          */
         uint8_t m_payload_length;

         /**
          * The extended payload length is either contained in the single byte field above,
          * or one of the multi-byte fields below.
          */
         typedef union
         {
            // If 126 the following 2 bytes interpreted as a 16-bit unsigned
            // integer are the payload length.
            uint16_t m_twoByteExtended_payload_length;

            // if 127 the following 8 bytes are interpreted as a 64 bit
            // unsigned integer
            uint64_t m_eightByteExtended_payload_length;
         } extendedPayloadLength;

         extendedPayloadLength m_extPayloadLength;

         // NOTE: Keep m_mask, and the message variables as the last variable
         //       defined to ensure the order of the initialized variables.
         //       This is a cruddy side affect to C++ initialization lists
         //       (must be initialized in the order defined).
         /**
          * mask for sending data to the websocket server.
          * Websocket Spec requires all data sent from the client to the server
          * to be masked.
          */
         static const uint8_t m_mask[];

         /**
          * Application data payload.  This is the actual message sent.
          */
         std::string m_applicationMessage;

         /**
          * buffer to contain the encoded or decoded message.
          */
         uint8_t m_messageBuffer[MAX_BUFFER_SIZE];

         /**
          * The number of bytes in the message
          * (most often less then the MAX_BUFFER_SIZE)
          */
         uint16_t m_numberOfMesssageBytes;
   };

}
#endif
