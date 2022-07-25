#include "../basicTypes.h"
#include "../util/BasicUtil.h"
#include "../util/EnumByName.h"

#include "MovingAverage.h"

namespace CecilStLabs
{

   MovingAverage::MovingAverage()
      : m_current(0),
        m_full(false)
   {
      clear();
   }

   MovingAverage::~MovingAverage()
   {
      clear();
   }

   void MovingAverage::clear()
   {
      for (uint8_t i = 0; i < MAXIMUM_VALUES; i++)
      {
         m_values[i] = 0;
      }

      m_current = 0;
      m_full = false;
   }

   void MovingAverage::add(uint16_t value)
   {
      m_values[m_current++] = value;

      if (m_current >= MAXIMUM_VALUES)
      {
         m_current = 0;
         m_full = true;
      }
   }

   bool MovingAverage::isFull()
   {
      return m_full;
   }

   uint16_t MovingAverage::calculate()
   {
      uint16_t integerAverage = 0;
      uint32_t integerSum = 0;

      // conditionally set the maximum number of values used to average the
      // values that populate the buffer.
      uint8_t max = m_current;
      if (true == isFull())
      {
         max = MAXIMUM_VALUES;
      }

      // sum up the total values.
      for (uint8_t i = 0; i < max; i++)
      {
         integerSum += m_values[i];
      }

      // average out the total values, dropping any fractional part.
      if (0 == max)
      {
         integerAverage = 0;
      }
      else
      {
         integerAverage = integerSum / max;
      }

      return integerAverage;
   }
}
