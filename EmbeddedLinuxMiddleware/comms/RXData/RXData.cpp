
#include <cstring>

#include "../../common/basicTypes.h"
#include "../../common/util/BasicUtil.h"
#include "../../common/LinkedList/SingleLink.h"

#include "RXData.h"

namespace CecilStLabs
{
   RXData::RXData()
      : m_numBytes(0)
   {
      memset(m_data, 0, MAX_RX_BYTES);
   }

   bool RXData::SetData(uint8_t* data, uint16_t numBytes)
   {
      bool success = false;

      // clear first
      Clear();

      if (NULL != data)
      {
         if (numBytes <= MAX_RX_BYTES)
         {
            m_numBytes = numBytes;
            memcpy(m_data, data, numBytes);

            success = true;
         }
      }

      return success;
   }

   void RXData::Clear()
   {
      m_numBytes = 0;
      memset(m_data, 0, MAX_RX_BYTES);
   }

   uint16_t RXData::GetNumBytes()
   {
      return m_numBytes;
   }

   void RXData::SetNumBytes(uint16_t numBytes)
   {
      m_numBytes = numBytes;
   }

   uint8_t* RXData::GetDataPtr()
   {
      return m_data;
   }
}
