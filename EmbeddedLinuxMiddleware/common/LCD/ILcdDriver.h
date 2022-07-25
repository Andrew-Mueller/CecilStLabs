#ifndef ILCDDRIVER_H
#define ILCDDRIVER_H

namespace CecilStLabs
{
   /**
    * Interface to LCD Drivers that can be used by the application layer.
    */
   class ILcdDriver
   {
      public:

         /**
          * Perform a soft reboot of the LCD to ensure it is returned to a known
          * good state.
          */
         virtual void reboot() = 0;

         /**
          * Clear the LCD of all characters.
          */
         virtual void clear() = 0;

         /**
          * Write the specified string of characters to the LCD.
          *
          * @param text The string of text to write to the LCD.
          */
         virtual void write(const std::string&  text) = 0;

         /**
          * Sets the contrast of the LCD from 0% (all faded out as the same color
          * as the background) to 100% (completely dark and indistinguishable squares).
          *
          * @param contrastPercent The contrast of the LCD from 0 to 100 percent.
          */
         virtual void setContrast(uint8_t contrastPercent) = 0;

         /**
          * Sets the backlight of the LCD from 0% (completely dark) to 100% as
          * bright as possible.
          *
          * @param backlightPercent 0 to 100 to represent the brightness of the
          *                         LCD's backlight.
          */
         virtual void setBacklight(uint8_t backlightPercent) = 0;

         /**
          * Saves the last written message to on board ram of the LCD.
          */
         virtual void save() = 0;

      protected:

         /**
          * define the default constructor, destructor, copy constructor and
          * assignment operator to ensure the interface on its own cannot be created.
          */

         ILcdDriver() { };
         virtual ~ILcdDriver() { };

         ILcdDriver(ILcdDriver& copy) { };

         ILcdDriver& operator= (const ILcdDriver& target) { return *this; };
   };
}

#endif
