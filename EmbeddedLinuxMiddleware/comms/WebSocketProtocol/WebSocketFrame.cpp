#include <iostream>
#include <string.h>

#include "../../common/basicTypes.h"
#include "../../common/util/EnumByName.h"
#include "../../common/DateTime/DateTime.h"
#include "../../common/util/BasicUtil.h"

#include "WebSocketFrame.h"

// define this to simulate breaking the websocket protocol which will force the
// server side to close our socket.
#undef TEST_BREAK_WEBSOCKETS

namespace CecilStLabs
{
   // using these simple hard coded mask bytes for now.  The could be
   // anything since the protocol sends the mask with each frame.
   const uint8_t WebSocketFrame::m_mask[] = {0x12, 0x34, 0x56, 0x78};

   WebSocketFrame::WebSocketFrame()
   : m_opcode(0),
     m_payload_length(0),
     m_applicationMessage(""),
     m_numberOfMesssageBytes(0)
   {
      memset(m_messageBuffer, 0, MAX_BUFFER_SIZE);
   }

   WebSocketFrame::~WebSocketFrame()
   {
      // TODO Auto-generated destructor stub
   }

   void WebSocketFrame::encode()
   {
      if (!m_applicationMessage.empty())
      {
         uint16_t byteIndex = 0;

         memset(m_messageBuffer, 0, MAX_BUFFER_SIZE);
         m_numberOfMesssageBytes = 0;

         // first byte needs to be opcode.
         // set the MSB (FIN) if this is the last message in the frame
         // opcode is just 0x1 (UTF8 Text Frame for the chat application)
         m_messageBuffer[byteIndex++] = (byte_t)FIN_BIT | (byte_t)websocket_utf8_text;

         // second byte is the mask bit and payload data
         // set the MSB to 1 to indicate the data is masked
         //
         // set the mask bit to 1 (all frames sent from client to server must be masked)
         // the last 7 bits of the second byte is the payload length (number of bytes in the payload)
         //  if 126, also use the following 2 bytes for payload length
         //  if 127, also use the following 8 bytes for the payload length
         // 0 - 125
         // 126 => 0 - 65535
         // 127 => 0 - 9223372036854775807

         //  multi-byte lengths are expressed in network byte order
#ifndef TEST_BREAK_WEBSOCKETS
         if (m_applicationMessage.length() + 1 < TWO_BYTE_LENGTH_LIMIT)
         {
            m_messageBuffer[byteIndex++] = MASK_BIT | (m_applicationMessage.length() + 1);
         }
         else if ((m_applicationMessage.length() >= TWO_BYTE_LENGTH_LIMIT) &&
                  (m_applicationMessage.length() < MAX_UINT16_VALUE))
         {
            m_messageBuffer[byteIndex++] = MASK_BIT | TWO_BYTE_LENGTH_LIMIT;
            m_messageBuffer[byteIndex++] = (uint8_t)(((m_applicationMessage.length() + 1) & 0xFF00) >> 8);
            m_messageBuffer[byteIndex++] = (uint8_t)((m_applicationMessage.length() + 1) & 0x00FF);
         }
         else
         {
            // TODO: encode 64 bit value length.
         }
#else
         // simulate a broken websocket by screwing up the size.
         m_messageBuffer[byteIndex++] = MASK_BIT | m_applicationMessage.length();
#endif

         // the next four bytes are the masking key
         m_messageBuffer[byteIndex++] = m_mask[0];
         m_messageBuffer[byteIndex++] = m_mask[1];
         m_messageBuffer[byteIndex++] = m_mask[2];
         m_messageBuffer[byteIndex++] = m_mask[3];

         // payload data (payload size specified in byte 2)
         uint8_t offset = byteIndex;

         // mask the message according to the algorithm
         // specified in the RFC 6455 pg 32
         for(uint16_t i = 0;
             (i < m_applicationMessage.length() + 1) && (i < (MAX_BUFFER_SIZE - offset));
             i++)
         {
            uint8_t j = i % 4;
            m_messageBuffer[offset + i] = (m_applicationMessage.c_str()[i] ^ m_mask[j]);
         }

         // add the null zero to mark the end of the string.
         m_messageBuffer[m_applicationMessage.length() + offset + 1] = '\0';

         // store the number of bytes in the encoded message.
         m_numberOfMesssageBytes = m_applicationMessage.length() + offset + 1;
      }
   }

   bool WebSocketFrame::decode()
   {
      bool success = true;
      uint16_t byteIndex = 0;
      uint16_t messageLength = 0;

      m_applicationMessage = "";

      if (m_messageBuffer[byteIndex++] != ((byte_t)FIN_BIT | (byte_t)websocket_utf8_text))
      {
         success = false;
      }

      // verify the mask bit is clear (all received websocket messages
      // are unmasked).
      // NOTE: byteIndex is purposely not incremented here because we look at it again to get the length.
      if (success &&
          (MASK_BIT == (m_messageBuffer[byteIndex] & MASK_BIT)))
      {
         // mask bit was incorrectly set.
         success = false;
      }
      else
      {
         // decode the message length
         messageLength = m_messageBuffer[byteIndex++] & ~(MASK_BIT);

         if (messageLength < TWO_BYTE_LENGTH_LIMIT)
         {
            // NOTE: thats it, this is the length (8 bytes)
         }
         else if (messageLength == TWO_BYTE_LENGTH_LIMIT)
         {
            // message length is 16 bit
            messageLength = m_messageBuffer[byteIndex++] << 8;
            messageLength |= m_messageBuffer[byteIndex++];
         }
         else
         {
            // TODO: messageLength is 64 bit and we only support messages that are 16 bit.
            // NOTE: messageLength variable is currently only 16 bit.
            success = false;
         }
      }

      if (success)
      {
         // extract the message from the remaining message buffer.
         m_applicationMessage.assign(std::string( reinterpret_cast<const char*>(m_messageBuffer + byteIndex), messageLength));
      }

      return success;
   }

   bool WebSocketFrame::buildCloseFrame(uint8_t* buffer, const uint8_t sizeOfBuffer)
   {
      bool success = false;

      if (NUM_BYTES_IN_CLOSE_FRAME == sizeOfBuffer)
      {
         memset(buffer, 0, NUM_BYTES_IN_CLOSE_FRAME);

         uint8_t i = 0;
         buffer[i++] = FIN_BIT | websocket_close;
         buffer[i++] = MASK_BIT;

         buffer[i++] = m_mask[0];
         buffer[i++] = m_mask[1];
         buffer[i++] = m_mask[2];
         buffer[i++] = m_mask[3];

         buffer[i++] = '\0';

         if (NUM_BYTES_IN_CLOSE_FRAME == i)
         {
            success = true;
         }
      }

      return success;
   }

   bool WebSocketFrame::buildPingFrame(uint8_t* buffer, const uint8_t sizeOfBuffer)
   {
      bool success = false;

      if (NUM_BYTES_IN_PING_FRAME == sizeOfBuffer)
      {
         uint8_t i = 0;
         buffer[i++] = FIN_BIT | websocket_ping;
         buffer[i++] = MASK_BIT;

         buffer[i++] = m_mask[0];
         buffer[i++] = m_mask[1];
         buffer[i++] = m_mask[2];
         buffer[i++] = m_mask[3];

         buffer[i++] = '\0';

         if (NUM_BYTES_IN_PING_FRAME == i)
         {
            success = true;
         }
      }

      return success;
   }
}

