/*
** DESCRIPTION:
** The CRC class calculates a 16-bit CRC. The class uses a table look up
** algorithm. The table is generated from an algorithm contained in:
** Numerical Recipes in C, The Art of Scientific Computing, Second Edition.
** Refer to Chapter 20, section 3.
*/


#ifndef CRC16_H
#define CRC16_H

#define LOBYTE(x)    ((uint8_t)((x) & 0xFF))
#define HIBYTE(x)    ((uint8_t)((x) >> 8))
#define HINIBBLE(x)  ((uint8_t)((x) >> 4))
#define LONIBBLE(x)  ((uint8_t)((x) & 0x0F))

/**
 * Uncomment this define if pre-calculated lookup tables are to be used
 * when calculating CRCs. NOTE: Using pre-calculated tables yields the
 * same result as not using pre-calculated tables.  The only difference
 * is that if lookup tables are used, a result will be calculated using
 * less processor cycles.  Unfortunately, more RAM is required.
 */
#define CRC_USE_LOOKUP_TABLES

/**
 * define if characters must be reversed for this CRC
 */
#undef CRC_REVERSE_CHARS

namespace CecilStLabs
{
   typedef uint32_t crcLength_t;

   class CRC16
   {
      public:

         /**
          * Default constructor. Sets the starting value to 0.
          */
         CRC16();

         /**
          * Constructor. Initializes start value to input parameter.
          */
         CRC16(uint16_t startValue);

         /**
          * Resets the CRC16 starting value.  The default
          * is to reset the value to 0.  A different starting value can
          * be specified if desired.  Starting a CRC at a non-zero value
          * is useful in detecting if an entire buffer including the CRC
          * value has been set to 0
          */
         void reset (uint16_t startValue = 0);

         /**
          * Includes the specified buffer into the existing CRC value
          */
         void append (uint8_t *pBuffer, crcLength_t length);

         /**
          * Returns the current CRC value.
          */
         uint16_t getCrc();

         /**
          * Byte-swaps the CRC value
          */
         uint16_t getSwappedCrc();

         /**
          * Compliments the CRC value
          */
         uint16_t getComplimentedCrc();

         /**
          * Byte-swaps and compliments the CRC value
          */
         uint16_t getSwappedAndComplimentedCrc();

         /**
          * Compares one CRC to another.  Returns 1 if equal, 0 if not equal
          */
         int operator== (const CRC16 &crc);

         /**
          *  Compares one CRC to another. Returns 1 if not equal, 0 if equal
          */
         int operator!= (const CRC16 &crc);

      protected:
         static const uint16_t NUM_CRC_TABLE_VALUES = 256;

         static const uint8_t NUM_REV_HELPER_CHARACTERS = 16;

         /**
          * Current calculated value of the 16-bit crc.
          */
         uint16_t m_crcValue16;

         #ifdef CRC_USE_LOOKUP_TABLES
            static const uint16_t CRC_TABLE[NUM_CRC_TABLE_VALUES];

            #ifdef CRC_REVERSE_CHARS
               static const uint8_t [NUM_CRC_TABLE_VALUES];
            #endif
         #else
            #ifdef CRC_REVERSE_CHARS
               static const uint8_t REV_CHAR_HELPER[NUM_REV_HELPER_CHARACTERS];
            #endif
         #endif
   };

};

#endif
