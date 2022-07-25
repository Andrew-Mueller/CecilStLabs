#ifndef NULLLCDDRIVER_H_
#define NULLLCDDRIVER_H_

namespace CecilStLabs
{

   /**
    * An empty LCD Driver that can be used instead of a true hardware
    * connected LCD.
    *
    * This helps keep higher level behavior common between similar applications
    * by ensuring a concrete implementation of the ILcdDriver interface.
    */
   class NullLCDDriver : public ILcdDriver
   {
      public:
         NullLCDDriver() { /* intentionally left blank */ };
         virtual ~NullLCDDriver() { /* intentionally left blank */ };

         /**
          * Perform a soft reboot of the LCD to ensure it is returned to a known
          * good state.
          */
         virtual void reboot() { /* intentionally left blank */ };

         /**
          * Clear the LCD of all characters.
          */
         inline void clear() { /* intentionally left blank */ };

         /**
          * Write the specified string of characters to the LCD.
          *
          * @param text The string of text to write to the LCD.
          */
         inline void write(const std::string& text) { /* intentionally left blank */ };

         /**
          * Sets the contrast of the LCD from 0% (all faded out as the same color
          * as the background) to 100% (completely dark and indistinguishable squares).
          *
          * @param contrastPercent The contrast of the LCD from 0 to 100 percent.
          */
         inline void setContrast(uint8_t contrastPercent) { /* intentionally left blank */ };

         /**
          * Sets the backlight of the LCD from 0% (completely dark) to 100% as
          * bright as possible.
          *
          * @param backlightPercent 0 to 100 to represent the brightness of the
          *                         LCD's backlight.
          */
         inline void setBacklight(uint8_t backlightPercent) { /* intentionally left blank */ };

         /**
          * Save the last written setting to lcd ram if available.
          */
         inline void save() { /* intentionally left blank */ };

   };


}
#endif
