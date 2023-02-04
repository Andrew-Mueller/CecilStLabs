#include <cstring>
#include <cstdlib>

// the standard library's string and stream operators
#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>
using namespace std;

// basic types
#include "basicTypes.h"
#include "util/EnumByName.h"
#include "DateTime/DateTime.h"

#include "logging/LogEntry.h"
#include "logging/ILogDriver.h"

#include "util/BasicUtil.h"

#include "guid.h"

namespace CecilStLabs
{
   GUID::GUID()
   {
      Zero();
   }

   GUID::GUID(guid_t& guid)
   {
      Set(guid);
   }

   GUID::GUID(string guid_str)
   {
      // parse the string representation of the guid
      // The guid string is expected to be 00000000-0000-0000-0000-000000000000
      bool parseSuccessful = Set(guid_str);

      // if the string version of the Guid is
      if (!parseSuccessful)
      {
         Zero();
      }
   }

   GUID::~GUID()
   {
      // intentionally left blank
   }

   void GUID::Zero()
   {
      memset((void*)&m_guid, 0, sizeof(m_guid));
   }

   bool GUID::Set(guid_t& guid)
   {
      // TODO: should some error checking occur here?
      memcpy(&m_guid, &guid, sizeof(guid_t));

      // NOTE: the return value here is strictly to match the overloaded Set
      //       method.
      return true;
   }

   bool GUID::Set(string guid_str)
   {
      bool success = false;
      string parsedOctet;

      size_t firstOctetPos = ParseOctetString(guid_str, '-', 0, parsedOctet);

      if (ParseOctetBytes(parsedOctet, m_guid.Data1, FIRST_OCTET_BYTES))
      {
         size_t secondOctetPos = ParseOctetString(guid_str, '-', firstOctetPos + 1, parsedOctet);

         if (ParseOctetBytes(parsedOctet, m_guid.Data2, SECOND_OCTET_BYTES))
         {
            size_t thirdOctetPos = ParseOctetString(guid_str, '-', secondOctetPos + 1, parsedOctet);

            if (ParseOctetBytes(parsedOctet, m_guid.Data3, THIRD_OCTET_BYTES))
            {
               size_t fourAOctetPos = ParseOctetString(guid_str, '-', thirdOctetPos + 1, parsedOctet);

               if (ParseOctetBytes(parsedOctet, m_guid.Data4, FOUR_A_OCTET_BYTES))
               {
                  parsedOctet = guid_str.substr((fourAOctetPos + 1), guid_str.length() - (fourAOctetPos + 1));

                  if (ParseOctetBytes(parsedOctet, m_guid.Data4 + FOUR_A_OCTET_BYTES, FOUR_B_OCTET_BYTES))
                  {
                     success = true;
                  }
               }
            }
         }
      }

      return success;
   }

   string GUID::ToString()
   {
      stringstream str;
      str << std::hex << std::setfill('0');
      str << std::setw(2) << static_cast<unsigned int>(m_guid.Data1[0]) << std::setw(2) << static_cast<unsigned int>(m_guid.Data1[1]) << std::setw(2) << static_cast<unsigned int>(m_guid.Data1[2]) << std::setw(2) << static_cast<unsigned int>(m_guid.Data1[3]) << "-"
          << std::setw(2) << static_cast<unsigned int>(m_guid.Data2[0]) << std::setw(2) << static_cast<unsigned int>(m_guid.Data2[1]) << "-"
          << std::setw(2) << static_cast<unsigned int>(m_guid.Data3[0]) << std::setw(2) << static_cast<unsigned int>(m_guid.Data3[1]) << "-"
          << std::setw(2) << static_cast<unsigned int>(m_guid.Data4[0]) << std::setw(2) << static_cast<unsigned int>(m_guid.Data4[1]) << "-"
          << std::setw(2) << static_cast<unsigned int>(m_guid.Data4[2]) << std::setw(2) << static_cast<unsigned int>(m_guid.Data4[3]) << std::setw(2) << static_cast<unsigned int>(m_guid.Data4[4])
          << std::setw(2) << static_cast<unsigned int>(m_guid.Data4[5]) << std::setw(2) << static_cast<unsigned int>(m_guid.Data4[6]) << std::setw(2) << static_cast<unsigned int>(m_guid.Data4[7]);

      return str.str();
   }

   size_t GUID::ParseOctetString(const string& parse_str,
                                 const char seperator,
                                 const size_t startPos,
                                 string& parsedValue)
   {
      parsedValue = "";

      size_t endPos = parse_str.find(seperator, startPos);
      if (string::npos != endPos)
      {
         parsedValue = parse_str.substr(startPos, endPos - (startPos));
      }

      return endPos;
   }

   bool GUID::ParseOctetBytes(const std::string& parse_str,
                              byte_t* destinationBuffer,
                              const uint8_t expectedNumBytes)
   {
      bool parseSuccess = false;
      char asciiHex[2] = {0, 0};

      if (parse_str.length() == (size_t)(expectedNumBytes * 2))
      {
         for (uint8_t i = 0; i < (expectedNumBytes * 2); i += 2)
         {
            asciiHex[0] = parse_str.c_str()[i];
            asciiHex[1] = parse_str.c_str()[i + 1];

            destinationBuffer[i / 2] = strtol(asciiHex, NULL, 16);
         }

         parseSuccess = true;
      }

      return parseSuccess;
   }

   bool GUID::operator== ( const GUID& rhs) const
   {
      return (this->m_guid.Data1[0] == rhs.m_guid.Data1[0] &&
              this->m_guid.Data1[1] == rhs.m_guid.Data1[1] &&
              this->m_guid.Data1[2] == rhs.m_guid.Data1[2] &&
              this->m_guid.Data1[3] == rhs.m_guid.Data1[3] &&

              this->m_guid.Data2[0] == rhs.m_guid.Data2[0] &&
              this->m_guid.Data2[1] == rhs.m_guid.Data2[1] &&

              this->m_guid.Data3[0] == rhs.m_guid.Data3[0] &&
              this->m_guid.Data3[1] == rhs.m_guid.Data3[1] &&

              this->m_guid.Data4[0] == rhs.m_guid.Data4[0] &&
              this->m_guid.Data4[1] == rhs.m_guid.Data4[1] &&
              this->m_guid.Data4[2] == rhs.m_guid.Data4[2] &&
              this->m_guid.Data4[3] == rhs.m_guid.Data4[3] &&
              this->m_guid.Data4[4] == rhs.m_guid.Data4[4] &&
              this->m_guid.Data4[5] == rhs.m_guid.Data4[5] &&
              this->m_guid.Data4[6] == rhs.m_guid.Data4[6] &&
              this->m_guid.Data4[7] == rhs.m_guid.Data4[7]);
   }

   bool GUID::operator!= (const GUID& rhs) const
   {
      return !(*this == rhs);
   }

   byte_t GUID::operator[](const unsigned int index) const
   {
      byte_t retByte = 0;

      if (index <= 3u)
      {
         retByte = this->m_guid.Data1[index];
      }
      else if (index >= 4u && index <= 5u)
      {
         retByte = this->m_guid.Data2[index - FIRST_OCTET_BYTES];
      }
      else if (index >= 6u && index <= 7u)
      {
         retByte = this->m_guid.Data3[index - (FIRST_OCTET_BYTES + SECOND_OCTET_BYTES)];
      }
      else if (index >= 8u && index <= 15u)
      {
         retByte = this->m_guid.Data4[index - (FIRST_OCTET_BYTES + SECOND_OCTET_BYTES + THIRD_OCTET_BYTES)];
      }
      else
      {
         // index is out of bound.
         // We never expect to be here so Halt the execution of the program.

         BasicUtil::Assert(false, __FILE__, __LINE__);
      }

      return retByte;
   }
}
