#include <string>
#include <iostream>
#include <sstream>
using namespace std;

#include "../basicTypes.h"
#include "../util/EnumByName.h"
#include "../DateTime/DateTime.h"

#include "../logging/LogEntry.h"
#include "../logging/ILogDriver.h"

#include "../util/BasicUtil.h"

#include "../DateTime/IClockDriver.h"

#include "../util/IAppender.h"
#include "../util/DebugPrint.h"
#include "../logging/LogEntry.h"

#include "../LinkedList/SingleLink.h"
#include "../RingBuffer/RingBuffer.h"
#include "../guid.h"

#include "../tinyxml/tinyxml2.h"
using namespace tinyxml2;

#include "IConfig.h"
#include "IConfigAction.h"
#include "ConfigActionWrite.h"

namespace CecilStLabs
{
   ConfigActionWrite::ConfigActionWrite()
   {
      // intentionally left blank.
   }

   ConfigActionWrite::~ConfigActionWrite()
   {
      // intentionally left blank.
   }

   bool ConfigActionWrite::execute(const XMLElement* root,
                                         std::string elementName,
                                         std::stringstream& elementValue)
   {
      bool success = false;

      XMLElement* child = NULL;

      if ((NULL != root) && !elementName.empty())
      {
         // TODO: casting here to remove const-ness
         //       i think I need to be using <static_cast> or something else
         child = (XMLElement*)root->FirstChildElement(elementName.c_str());

         if (NULL != child)
         {
            child->SetValue(elementValue.str().c_str());
            success = true;
         }
      }

      return success;
   }
}

