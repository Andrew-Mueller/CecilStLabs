
#ifndef ILCDCOMMUNICATION_H_
#define ILCDCOMMUNICATION_H_

namespace CecilStLabs
{
   /**
    * The communication method used to talk to the the ILcdDriver.  The abstraction
    * exists because a single LCDDriver may have multiple ways to communicate with it,
    * for example, RS232, USB, or i2C.
    */
   class ILcdCommunication
   {
      public:

         /**
          * The LCD communication interface is intentionally blank to provide a
          * layer of abstraction between the LCD Driver and the method used to
          * communicate between the micro and the LCDDriver.  For Example, the
          * LCD Driver may be specific for a CrystalFontz CFA632 LCD Module, but
          * is connected via i2c.
          */

         /**
          * Sends the specified data to the LCD using this classes specialized
          * communication method.
          *
          * @param data The pointer to the bytes to send to the LCD.
          * @param length The number of bytes in 'data'.
          */
         virtual void send(uint8_t* data, uint16_t length) = 0;

      protected:
         /**
          * define the default constructor, destructor, copy constructor and
          * assignment operator to ensure the interface on its own cannot be created.
          */

         ILcdCommunication() {};
         virtual ~ILcdCommunication() {};

         ILcdCommunication(ILcdCommunication& copy) { };

         ILcdCommunication& operator= (const ILcdCommunication& target) { return *this; };
   };
}

#endif
