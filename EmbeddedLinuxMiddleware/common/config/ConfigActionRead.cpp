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
#include "ConfigActionRead.h"


namespace CecilStLabs
{
   ConfigActionRead::ConfigActionRead()
   {
      // intentionally left blank
   }

   ConfigActionRead::~ConfigActionRead()
   {
      // intentionally left blank
   }

   // TODO: this is specialized for an element that is expected to only
   //       have a single value, and no children.
   // TODO: need to create a second version that returns the collection of elements?
   //       if the child is not a single value element.
   bool ConfigActionRead::execute(const XMLElement* root,
                                        string elementName,
                                        stringstream& buffer)
   {
      bool success = false;

      XMLElement* child = NULL;
      buffer.str("");

      if ((NULL != root) &&
          !elementName.empty())
      {
         // cast the first child element to the (XMLElement*) to remove const-ness
         // i think I need to be using <static_cast> or something else
         child = (XMLElement*)root->FirstChildElement(elementName.c_str());

         if (NULL != child)
         {
            // set the value of the element to the string stream's current value.
            // This is used to aid in converting the read value into the proper
            // member
            buffer.clear();
            if (child->GetText() != NULL)
            {
               buffer.str(child->GetText());
               success = true;
            }
         }
      }

      return success;
   }
}
