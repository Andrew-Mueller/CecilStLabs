#ifndef IWATCHDOG_H_
#define IWATCHDOG_H_

namespace CecilStLabs
{

   /**
    * Watchdog interface for common code to utilize generically such as initialization,
    * enabling, disabling, and kicking the Watchdog Peripheral.
    */
   class IWatchdog
   {
      public:

         /**
          * Initialize the Watchdog Timer Peripheral
          */
         void initialize();

         /**
          * Enable the Watchdog Timer Peripheral, while enabled it is expected that
          * the Watchdog peripheral needs to be kicked to prevent the microprocessor
          * from being reset.
          */
         void enable();

         /**
          * Disable the Watchdog Timer Peripheral so it does not requiring
          * kicking for the microprocessor to not reset.
          */
         void disable();

         /**
          * Reset the Watchdog Timer Peripheral.  The Watchdog requires being
          * kicked periodically or the peripheral will reset the microprocessor.
          */
         void kick();

      protected:

         /**
          * define the constructor, copy constructor, destructor, and assignment
          * operator to ensure that an instance to the interface cannot be created
          * without a concrete class implementation..
          */
         IWatchdog() {};
         IWatchdog(IWatchdog& copy __attribute__((unused)) ) { };
         virtual ~IWatchdog() {};
         IWatchdog& operator=(IWatchdog& rhs __attribute__((unused)) ) { return rhs; };
   };

}
#endif
