//#include <errno.h>
//#include <string.h>

//#include <iostream>
//#include <sstream>
//#include <string>
using namespace std;

#include "../../common/tersoTypes.h"
#include "../../common/util/EnumByName.h"
#include "../../common/DateTime/DateTime.h"
#include "../../common/DateTime/IClockDriver.h"
#include "../../common/util/TersoUtil.h"
#include "../../common/guid.h"

#include "../../common/LinkedList/SingleLink.h"
#include "../../common/LinkedList/SingleLinkedList.h"
#include "../../common/MemoryPool/MemoryPool.h"

#include "../../common/tinyxml/tinyxml2.h"
using namespace tinyxml2;

#include "../../common/logging/LogEntry.h"
#include "../../common/logging/ILogDriver.h"
#include "../../common/logging/emptyLogDriver.h"
#include "../../common/logging/ILoggable.h"

#include "../../common/config/IConfig.h"
#include "../../common/util/NameValuePair.h"

#include "../../common/config/JetstreamAgentConfigSection.h"

#include "../../common/InternalEvent/IInternalEvent.h"
#include "../../common/InternalEvent/IInternalEventHandler.h"
#include "../../common/InternalEvent/InternalEventRegistry.h"
#include "../../common/InternalEvent/SetConfigInternalEvent.h"

#include "../jetstreamMessages.h"
#include "../Events/jetstreamEvent.h"
#include "../Events/CommandCompletionEvent.h"

#include "Command.h"
#include "SetConfigValuesCommand.h"

namespace Terso
{

   SetConfigValuesCommand::SetConfigValuesCommand(IConfig& config,
                                                  IClockDriver* clockDriver,
                                                  std::string deviceAccessKey,
                                                  std::string deviceSerialNumber)
      : Command(deviceAccessKey, deviceSerialNumber, clockDriver),
        m_configValuePool(m_configValues, MAX_NUM_PARAMETERS),
        m_configuration(config)
   {
      clearArgumentList();

      // clear out all of the configuration values.
      for (uint8_t i = 0; i < MAX_NUM_PARAMETERS; i++)
      {
         m_configValues[i].setName(std::string(""));
         m_configValues[i].setValue(std::string(""));
      }

   }

   SetConfigValuesCommand::~SetConfigValuesCommand()
   {
      NameValuePair* configValue = (NameValuePair*)m_argumentList.first();

      while (NULL != configValue)
      {
         configValue->setName(std::string(""));
         configValue->setValue(std::string(""));

         configValue = (NameValuePair*)m_argumentList.remove(configValue);

         if (NULL != configValue)
         {
            m_configValuePool.free(configValue);
         }
         else
         {
            this->getLogDriver()->Log("Error removing config value from command's linked list.", LoggingDebug, "");
         }

         configValue = (NameValuePair*)m_argumentList.first();
      }
   }

   void SetConfigValuesCommand::addArgument(std::string name,
                                            std::string value)
   {
      NameValuePair* configValue = m_configValuePool.allocate();

      if (configValue != NULL)
      {
         configValue->setName(name);
         configValue->setValue(value);

         Command::addArgument(configValue);
      }
   }

   CommandCompletionEvent* SetConfigValuesCommand::action()
   {       
      // clear out any previous data.
      m_cmdCompletionEvent.clear();
      m_cmdCompletionEvent.setCommandID(m_commandID);
      
      // take first one off end of list,
      // they are added at head so this acts like a FIFO
      NameValuePair* configValue = (NameValuePair*)m_argumentList.removeFromEnd();
      while (NULL != configValue)
      {
         if( NULL != m_event )
         {            
            SetConfigInternalEvent evt( configValue->getName(), configValue->getValue() );
            m_event->sendEvent( evt );
         }
            
         m_configuration.setValueByName(configValue->getName(), configValue->getValue());
        
         configValue = (NameValuePair*)m_argumentList.removeFromEnd();
      }

      return &m_cmdCompletionEvent;
   }
}
