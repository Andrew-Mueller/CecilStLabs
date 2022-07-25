#include <iostream>
using namespace std;

#include "../basicTypes.h"
#include "../util/BasicUtil.h"
#include "../util/EnumByName.h"
#include "../guid.h"

#include "../DateTime/DateTime.h"

#include "../LinkedList/SingleLink.h"

#include "NameValuePair.h"

namespace CecilStLabs
{
   NameValuePair::NameValuePair()
      : SingleLink(),
        m_name(""),
        m_value("")
   {
      // intentionally left blank.
   }

   NameValuePair::NameValuePair(std::string name, std::string value)
      : SingleLink(),
        m_name(name),
        m_value(value)
   {
      // intentionally left blank.
   }

   NameValuePair::~NameValuePair()
   {
      // intentionally left blank.
   }
}
