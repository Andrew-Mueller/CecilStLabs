#ifndef IGPIO_H_
#define IGPIO_H_

namespace CecilStLabs
{
   /**
    * GPIO can be configured as either input or output.
    */
   enum eGPIODirection
   {
      Input,
      Output
   };

   /**
    * Enumerate the GPIO Ports available.
    */
   enum ePort
   {
      Port0 = 0,
      Port1 = 1,
      Port2 = 2,
      Port3 = 3
   };

   /**
    * Enumerate the GPIO pins
    */
   enum ePin
   {
      Pin0  = 0x00000001,
      Pin1  = 0x00000002,
      Pin2  = 0x00000004,
      Pin3  = 0x00000008,
      Pin4  = 0x00000010,
      Pin5  = 0x00000020,
      Pin6  = 0x00000040,
      Pin7  = 0x00000080,
      Pin8  = 0x00000100,
      Pin9  = 0x00000200,
      Pin10 = 0x00000400,
      Pin11 = 0x00000800,
      Pin12 = 0x00001000,
      Pin13 = 0x00002000,
      Pin14 = 0x00004000,
      Pin15 = 0x00008000,
      Pin16 = 0x00010000,
      Pin17 = 0x00020000,
      Pin18 = 0x00040000,
      Pin19 = 0x00080000,
      Pin20 = 0x00100000,
      Pin21 = 0x00200000,
      Pin22 = 0x00400000,
      Pin23 = 0x00800000,
      Pin24 = 0x01000000,
      Pin25 = 0x02000000,
      Pin26 = 0x04000000,
      Pin27 = 0x08000000,
      Pin28 = 0x10000000,
      Pin29 = 0x20000000,
      Pin30 = 0x40000000,
      Pin31 = 0x80000000
   };

   /**
    * Interface for hardware GPIO Driver
    */
   class IGPIODriver
   {
      public:

         /**
          * Set the direction of the specified pins on the specified port.
          * NOTE: all pins are initially set low (0).
          *
          * @param port The GPIO Port to set the pin direction for.
          * @param pins Logically or'd ePin enumerations to mark the pins to setup.
          * @param direction Either Input or Output.
          */
         virtual void Setup(ePort port, uint8_t pins, eGPIODirection direction) = 0;

         /**
          * Sets the value of the specified output pins to high (1)
          *
          * @param port The GPIO Port to turn the pins on.
          * @param pins Logically or'd ePin enum to turn the pins on.
          */
         virtual void Set(ePort port, uint8_t pins) = 0;

         /**
          * Clears the value of the specified output pins to low (0).
          *
          * @param port The GPIO Port to turn the pins off.
          * @param pins Logically or'd ePin enum to turn the pins off.
          */
         virtual void Clear(ePort port, uint8_t pins) = 0;

         /**
          * Reads the value of the specified input pins.
          *
          * @param port the GPIO Port to read the input pins on.
          * @returns The current value of the input pins.
          */
         virtual uint8_t Read(ePort port) = 0;

         /**
          * Reads the value of the specified input pin on the specified port.
          *
          * @param port the GPIO Port to read the input pins on.
          * @param pin the pin to read
          * @returns The current value of the input pin.
          */
         virtual uint8_t ReadPin(ePort port, uint8_t pin) = 0;

      protected:
         // NOTE: interface constructor, copy constructor, and destructor are
         //       defined here to ensure an instance of the interface cannot be
         //       created.
         IGPIODriver() {};
         IGPIODriver(IGPIODriver& copy) { (void)copy; };
         virtual ~IGPIODriver() {};
   };
}

#endif
