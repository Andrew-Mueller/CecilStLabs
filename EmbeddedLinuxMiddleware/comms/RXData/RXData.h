#ifndef RXDATA_H
#define RXDATA_H

namespace CecilStLabs
{
   /**
    * Generic data received that can be added to a linked list, and therefore
    * added to some of the other data structures (Memory Pool, etc.)
    */
   class RXData : SingleLink
   {
      public:

         /**
          * Creates an empty RXData object.
          */
         RXData();

         /**
          * Copies the data at the specified location into this RXData object.
          * @param data Location of data to copy
          * @param numBytes The number of bytes in the data specified (must be less than MAX_RX_BYTES).
          *
          * @returns True if the data was set, False if the number of bytes
          *          exceeded MAX_RX_BYTES or the specified data was invalid.
          */
         bool SetData(uint8_t* data, uint16_t numBytes);

         /**
          * Sets the data to all zeros and the number of bytes to zero.
          */
         void Clear();

         /**
          * Getter for the number of bytes in this RXData object.
          * @return The number of bytes in the data.
          */
         uint16_t GetNumBytes();

         /**
          * Updates the number of bytes based on the previously set data.
          * Intended to be used if the data is updated through GetDataPtr().
          * @param numBytes The number of bytes to set for this receive object.
          */
         void SetNumBytes(uint16_t numBytes);

         /**
          * Getter for the data pointer in this RXData object. (Not copied).
          */
         uint8_t* GetDataPtr();

         /**
          * Destroy the RXData object.
          */
         virtual ~RXData() {};


         /**
          * Maximum number of bytes that can be inserted into a single RXData
          * object.
          * NOTE: public so objects using RXData can utilize this constant.
          */
         static const uint16_t MAX_RX_BYTES = 1024;

      private:

         /**
          * The number of bytes in the buffer.
          */
         uint16_t m_numBytes;

         /**
          * Storage of the bytes received.
          */
         uint8_t m_data[MAX_RX_BYTES];
   };
}
#endif
