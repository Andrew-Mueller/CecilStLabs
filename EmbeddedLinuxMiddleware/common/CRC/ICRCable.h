#ifndef ICRCABLE_H_
#define ICRCABLE_H_

namespace CecilStLabs
{
   /**
    * Interface of a class that we would like to calculate the CRC on all of the
    * data members for.  It is the responsibility of the implementation to append
    * the desired data members to a CRC16 and maintain the current CRC16 in memory.
    */
   class ICRCable
   {
      public:

         /**
          * Retrieve the 16 bit CRC for the object that realizes this CRCable
          * interface.
          *
          * @return sixteen bit CRC for the object's data.
          */
         virtual uint16_t getCRC16() = 0;

      protected:

         // define the constructor, copy constructor, destructor and assignment
         // operator to ensure the Interface (pure abstract class) cannot be
         // instantiated on its own.  Instead it must be realized by a concrete
         // class that implements this interface..
         ICRCable() {};
         ICRCable(ICRCable& crcable __attribute((unused))) {};
         virtual ~ICRCable() {}
         ICRCable& operator=(const ICRCable& rhs __attribute__((unused))); { return *this; };

   };

}
#endif
