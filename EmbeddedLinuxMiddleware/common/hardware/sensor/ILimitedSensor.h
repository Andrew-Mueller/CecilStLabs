#ifndef ILIMITEDSENSOR_H_
#define ILIMITEDSENSOR_H_

namespace CecilStLabs
{
   class ILimitedSensor
   {

      protected:
         ILimitedSensor() { };
         ILimitedSensor(ILimitedSensor& sensor __attribute__((unused))) {};

         virtual ~ILimitedSensor() { };
   };
}

#endif
