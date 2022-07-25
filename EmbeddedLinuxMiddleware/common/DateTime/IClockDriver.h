#ifndef ICLOCKDRIVER_H
#define ICLOCKDRIVER_H

namespace CecilStLabs
{

   /**
    * Interface for the Hardware clock driver used to set the system time and,
    * if needed, also set the real-time clock.
    *
    * TODO: Move this IClockDriver from the DateTime folder to the hardware folder.
    */
   class IClockDriver
   {
      public:

         /**
          * Sets the system time specific to the hardware architecture.
          *
          * @param currentTime The date and time to set the hardware's clock to.
          * @return True if the time was set correctly, False if the hardware
          *         clock could not be set.
          */
         virtual bool set(DateTime& currentTime) = 0;

         /**
          * Gets the current time from the hardware clock.
          *
          * @return The current date and time.
          */
         virtual DateTime getCurrentTime() = 0;

      protected:

         /**
          * Default constructor, copy constructor, destructor and assignment
          * operator are protected to make sure this is a proper interface.
          */
         IClockDriver() {};
         IClockDriver(IClockDriver &copy __attribute__((unused))) {};
         virtual ~IClockDriver() {};
         IClockDriver& operator=(IClockDriver& rhs __attribute__((unused))) { return rhs; };
   };

}
#endif
