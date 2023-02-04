#ifndef ISENSOR_H_
#define ISENSOR_H_

namespace CecilStLabs
{
   /**
    * Interface for a sensor in the system that can be read from.
    * Inherits from single link so it can be put into the single linked list of
    * sensors.
    */
   class ISensor : public SingleLink
   {
      public:

         /**
          * Read the current value of the sensor.
          *
          * @return integer value representing the reading from the sensor.
          */
         virtual int16_t read() = 0;

         /**
          * Retrieves the name of the sensor as a string
          *
          * @return String containing the name of the sensor.
          */
         virtual std::string getName() = 0;

         /**
          * Retrieves the ignored state of the sensor.  The implementation of
          * 'how' a specific sensor is ignored, and even 'what it means' for a
          * sensor to be ignored is defined by the implementation of the sensor.
          *
          * @return True if the sensor reading should be ignored, False if the
          *         reading is valid.
          */
         virtual bool isIgnored() = 0;

      protected:

         /**
          * NOTE: define the default constructor, copy constructor, destructor,
          *       and assignment operator so the compiler doesn't provide defaults
          *       and the interface cannot be instantiated without a concrete
          *       instance.
          */
         ISensor() {};
         ISensor(ISensor& copy __attribute__((unused))) : SingleLink() {};
         virtual ~ISensor() {};
         ISensor& operator=(ISensor& rhs __attribute__((unused))) { return rhs; };
   };
}

#endif
