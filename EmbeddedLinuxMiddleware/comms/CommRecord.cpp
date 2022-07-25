#include <string>
using namespace std;

#include "../common/basicTypes.h"
#include "../common/util/EnumByName.h"
#include "../common/DateTime/DateTime.h"
#include "../common/util/BasicUtil.h"

#include "../common/CRC/CRC16.h"

#include "CommRecord.h"

namespace CecilStLabs
{
   CommRecord::CommRecord()
   : m_id(0),
     m_method(POST),
     m_url(""),
     m_message(""),
     m_messageID(0),
     m_timestamp(),
     m_CRC16(0)
   {
      // intentionally left blank.
   }

   CommRecord::CommRecord(HTTP_Method_e method,
                          std::string url,
                          std::string message,
                          uint32_t messageID,
                          DateTime timestamp)
      : m_id(0),
        m_method(method),
        m_url(url),
        m_message(message),
        m_messageID(messageID),
        m_timestamp(timestamp),
        m_CRC16(0)
   {
      // update the crc with the constructed values.
      calculateCRC();
   }

   CommRecord::~CommRecord()
   {
      // intentionally left blank.
   }

   bool CommRecord::operator!=(const CommRecord &record) const
   {
      if ((m_id != record.m_id) ||
          (m_method != record.m_method) ||
          (m_url != record.m_url) ||
          (m_message != record.m_message) ||
          (m_essageID != record.m_messageID) ||
          (m_timestamp != record.m_timestamp) ||
          (m_CRC16 != record.m_CRC16))
      {
         return true;
      }

      return false;
   }

   bool CommRecord::operator==(const CommRecord &record) const
   {
      if (*this != record)
      {
         return false;
      }

      return true;
   }

   void CommRecord::zero()
   {
      m_id = 0;
      m_method = POST;
      m_url = "";
      m_message = "";
      m_messageID = 0;
      m_timestamp.setTimeValue(0, 0, 0, 1, 1, 0);
      m_CRC16 = 0;
   }

   void CommRecord::calculateCRC()
   {
      // calculate the CRC 16 of the message and timestamp by appending the
      // various parts pf
      CRC16 crc(0);

      uint32_t tempEnum = m_method;
      crc.append((uint8_t*)&tempEnum, sizeof(uint32_t));

      crc.append((uint8_t*)m_url.c_str(), m_url.length());

      crc.append((uint8_t*)m_message.c_str(), m_message.length());

      tempEnum = m_messageID;
      crc.append((uint8_t*)&tempEnum, sizeof(uint32_t));

      // temporary two byte value (could be reused if other members are added).
      uint16_t twoByte = m_timestamp.getYear();
      crc.append((uint8_t*)&twoByte, 2);

      // temporary one byte variable reused below.
      uint8_t oneByte = m_timestamp.getMonth();
      crc.append((uint8_t*)&oneByte, 1);

      oneByte = m_timestamp.getDay();
      crc.append((uint8_t*)&oneByte, 1);

      oneByte = m_timestamp.getHour();
      crc.append((uint8_t*)&oneByte, 1);

      oneByte = m_timestamp.getMinute();
      crc.append((uint8_t*)&oneByte, 1);

      oneByte = m_timestamp.getSecond();
      crc.append((uint8_t*)&oneByte, 1);

      m_CRC16 = crc.getCrc();
   }
}
