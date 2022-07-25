#ifndef MOVINGAVERAGE_H_
#define MOVINGAVERAGE_H_

namespace CecilStLabs
{
   /**
    * Used to store a number of values as they are added and calculate the
    * average of those values.
    */
   class MovingAverage
   {
      public:

         /**
          * Create the initially empty moving average object
          */
         MovingAverage();

         /**
          * Release all resources used by the moving average object.
          */
         virtual ~MovingAverage();

         /**
          * Clears all of the values out of the moving average.
          */
         void clear();

         /**
          * Adds the specified value into the next moving average buffer.
          *
          * @param value The new value to add to the moving average buffer.
          */
         void add(uint16_t value);

         /**
          * Returns TRUE if the buffer is full, FALSE if there is still room.
          *
          * @return TRUE if the buffer is full, FALSE if there is still room.
          */
         bool isFull();

         /**
          * Calculates the average of the values in the buffer.
          *
          * @return Current unsigned integer average of the values in the buffer
          *         (fractional part is dropped).
          */
         uint16_t calculate();

      protected:
         /**
          * Maximum number of values in the moving average array.
          */
         static const uint8_t MAXIMUM_VALUES = 50;

         /**
          * Concrete array of values in the moving average.
          */
         uint16_t m_values[MAXIMUM_VALUES];

         /**
          * Index of the last value added to the moving average.  This value will
          * always be between 0 and MAXIMUM_VALUES
          */
         uint8_t m_current;

         /**
          * Latches to true after the values array is filled.
          */
         bool m_full;
   };

}
#endif
