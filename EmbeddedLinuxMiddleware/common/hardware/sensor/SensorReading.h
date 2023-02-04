#ifndef SENSORREADING_H_
#define SENSORREADING_H_

namespace CecilStLabs
{
   /**
    * Object containing the sensor name, reading value, and the timestamp when
    * the reading is taken.
    */
   class SensorReading : public SingleLink
   {
      public:
         SensorReading();

         /**
          * Create the sensor reading with the timestamp, sensor name, and
          * reading value.
          *
          * @param timestamp Timestamp when the reading was taken.
          * @param name Name of the sensor the reading was taken from.
          * @param value The reading from the Sensor.
          */
         SensorReading(DateTime timestamp, std::string name, float value);

         /**
          * Create the sensor reading from the SensorReading object specified.
          *
          * @param copy Sensor reading to get the name, timestamp, and value, from.
          */
         SensorReading(SensorReading& copy);

         /**
          * Release any resources consumed from this Sensor Reading object.
          */
         virtual ~SensorReading();

         // getter and setter for the sensor reading timestamp.
         inline DateTime& getTimestamp() { return m_readingTimestamp; };
         inline void setTimestamp(DateTime& timestamp) { m_readingTimestamp = timestamp; };

         // getter and setter for the sensor reading name
         inline std::string& getName() { return m_sensorName; };
         inline void setName(std::string& name) { m_sensorName = name; };

         // getter and setter for the sensor reading value
         inline float getValue() const { return m_readingValue; };
         inline void setValue(const float value) { m_readingValue = value; };

         /**
          * Assign the values of the specified Sensor Reading to this sensor reading.
          *
          * @param rhs SensorReading to copy values from.
          * @return SensorReading with the same values as the reading specified.
          */
         SensorReading& operator=(const SensorReading& rhs);

         // equality and not equal logical operators.
         bool operator==(const SensorReading& rhs) const;
         bool operator!=(const SensorReading& rhs) const;

      protected:

         /**
          * The timestamp when the sensor was read from.
          */
         DateTime m_readingTimestamp;

         /**
          * The name of the sensor that was read from.
          */
         std::string m_sensorName;

         /**
          * The value read from the sensor.
          */
         float m_readingValue;
   };

}
#endif
