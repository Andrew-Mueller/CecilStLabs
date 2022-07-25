#ifndef GUID_H
#define GUID_H

namespace CecilStLabs
{
   /**
    * The Globally Unique Identifier (GUID) is a 16-byte (128-bit) number.
    * The Guid data structure as it is represented in memory.
    */
   typedef struct tag_guid
   {
       /**
        * First Guid Data Group
        */
       byte_t Data1[4];

       /**
        * Second Guid Data Group
        */
       byte_t Data2[2];

       /**
        * Third Guid Data Group
        */
       byte_t Data3[2];

       /**
        * Forth Guid Data Group
        */
       byte_t Data4[8];

   } guid_t;

   /**
    * The Globally Unique Identifier object.
    */
   class GUID
   {
      public:

         /**
          * Create an empty Globally Unique Identifier object.
          */
         GUID();

         /**
          * Create a GUID with the specified GUID value.
          */
         GUID(guid_t& guid);

         /**
          * Create a GUID with the string representation of the GUID.
          */
         GUID(std::string guid);

         /**
          * Release the resources required by the GUID.
          */
         virtual ~GUID();

         /**
          * Load the GUID with all zeros.
          */
         void Zero();

         /**
          * Sets the contents of the GUID to the specified GUID structure value.
          */
         bool Set(guid_t& guid);

         /**
          * Sets the contents of the GUID with the string representation of the GUID.
          */
         bool Set(std::string guid);

         /**
          * Create a string representation of the GUID.
          */
         std::string ToString();

         /**
          * Comparison and inequality operators for checking guids.
          */
         bool operator== (const GUID& rhs) const;
         bool operator!= (const GUID& rhs) const;

         /**
          * Square bracket operator for indexing into the bytes of the Guid
          *
          * @param index The subscript index
          */
         byte_t operator[](const unsigned int index) const;

         static const uint8_t FIRST_OCTET_BYTES = 4;
         static const uint8_t SECOND_OCTET_BYTES = 2;
         static const uint8_t THIRD_OCTET_BYTES = 2;
         static const uint8_t FOUR_A_OCTET_BYTES = 2;
         static const uint8_t FOUR_B_OCTET_BYTES = 6;
         static const uint8_t FOURTH_OCTET_BYTES = FOUR_A_OCTET_BYTES + FOUR_B_OCTET_BYTES;

      protected:

         /**
          * GUID data bytes.  The memory representation of guid data.
          */
         guid_t m_guid;

         /**
          * Parse an octet substring from the string containing the guid.
          *
          * @param parse_str String containing the guid in the format 00000000-0000-0000-0000-000000000000
          * @param separator The character that separates the octet.
          * @param startPos Octet start position of the octet.
          * @param parsedValue The octet portion of the string.
          */
         virtual size_t ParseOctetString(const std::string& parse_str,
                                         const char seperator,
                                         const size_t startPos,
                                         std::string& parsedValue);

         /**
          * Parse the specified octet string into the desired destination byte buffer.
          *
          * @param parse_str String ontaining the ascii representation of hex bytes to parse.
          * @param destinationBuffer Destination buffer of bytes to place the converted bytes into.
          * @param expectedNumBytes Number of bytes in the string to convert.
          *        This should be the size of the destinationBuffer.
          */
         virtual bool ParseOctetBytes(const std::string& parse_str,
                                      byte_t* destinationBuffer,
                                      const uint8_t expectedNumBytes);
   };

}
#endif
