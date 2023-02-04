#ifndef ISENSORCONVERTED_H_
#define ISENSORCONVERTED_H_

namespace CecilStLabs
{
   class ISensorConverted : public ISensor
   {
      public:

         /**
          * Read the converted value of the sensor.
          *
          * @return The sensor reading converted by the converter implementation.
          */
         virtual float readConverted() = 0;

      protected:
         /**
          * NOTE: define the default constructor, copy constructor, destructor,
          *       and assignment operator so the compiler doesn't provide defaults
          *       and the interface cannot be instantiated without a concrete
          *       instance.
          */
         ISensorConverted() {};
         ISensorConverted(ISensorConverted& copy __attribute__((unused))) : ISensor() {};
         virtual ~ISensorConverted() {};
         ISensorConverted& operator=(ISensorConverted& rhs __attribute__((unused))) { return rhs; };
   };
}

#endif
