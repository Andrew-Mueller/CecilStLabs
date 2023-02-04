#include <algorithm>
#include <string>
using namespace std;

#include "../basicTypes.h"

#include "EnumByName.h"

namespace CecilStLabs
{
   EnumByName::EnumByName(uint32_t type, std::string name)
      : m_type(type),
        m_name(name)
   {
      // intentionally left blank.
   }

   EnumByName::~EnumByName()
   {
      // intentionally left blank.
   }

   bool EnumByName::isEnumType(std::string name) const
   {
      bool isMatch = false;

      // convert the member's name string to lower case
      std::string lowerName(m_name);
      std::transform(lowerName.begin(), lowerName.end(), lowerName.begin(), ::tolower);

      // convert the input name to the lower case
      std::string inputName(name);
      std::transform(inputName.begin(), inputName.end(), inputName.begin(), ::tolower);

      if (0 == inputName.compare(lowerName))
      {
         isMatch = true;
      }

      return isMatch;
   }
}
