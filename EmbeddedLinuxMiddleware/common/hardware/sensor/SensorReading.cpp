#include "../../basicTypes.h"
#include "../../util/EnumByName.h"
#include "../../DateTime/DateTime.h"
#include "../../util/BasicUtil.h"

#include "../../LinkedList/SingleLink.h"

#include "SensorReading.h"

namespace CecilStLabs
{

   SensorReading::SensorReading()
      : m_readingTimestamp(DateTime()),
        m_sensorName(""),
        m_readingValue(0)
   {
      // intentionally left blank.

   }

   SensorReading::SensorReading(DateTime timestamp, std::string name, float value)
      : m_readingTimestamp(timestamp),
        m_sensorName(name),
        m_readingValue(value)
   {
      // intentionally left blank.
   }

   SensorReading::SensorReading(SensorReading& copy)
               : m_readingTimestamp(copy.m_readingTimestamp),
                 m_sensorName(copy.m_sensorName),
                 m_readingValue(copy.m_readingValue)
   {
      // intentionally left blank.
   };

   SensorReading::~SensorReading()
   {
      // intentionally left blank.
   }

   SensorReading& SensorReading::operator=(const SensorReading& rhs)
   {
      m_readingTimestamp = rhs.m_readingTimestamp;
      m_sensorName = rhs.m_sensorName;
      m_readingValue = rhs.m_readingValue;

      return *this;
   }

   bool SensorReading::operator==(const SensorReading& rhs) const
   {
      if ((this->m_readingTimestamp == rhs.m_readingTimestamp) &&
          (this->m_sensorName == rhs.m_sensorName) &&
          (this->m_readingValue == rhs.m_readingValue))
      {
         return true;
      }

      return false;
   }

   bool SensorReading::operator!=(const SensorReading& rhs) const
   {
      if ((this->m_readingTimestamp == rhs.m_readingTimestamp) &&
                (this->m_sensorName == rhs.m_sensorName) &&
                (this->m_readingValue == rhs.m_readingValue))
      {
         return false;
      }

      return true;
   }

}
