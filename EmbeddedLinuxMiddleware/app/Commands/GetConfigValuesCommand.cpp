using namespace std;


#include "../../common/tersoTypes.h"               //lint !e761
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
#include "../../common/InternalEvent/IInternalEvent.h"
#include "../../common/InternalEvent/IInternalEventHandler.h"
#include "../../common/InternalEvent/InternalEventRegistry.h"

#include "../jetstreamMessages.h"
#include "../Events/jetstreamEvent.h"
#include "../Events/CommandCompletionEvent.h"

#include "Command.h"
#include "GetConfigValuesCommand.h"

namespace Terso
{

   GetConfigValuesCommand::GetConfigValuesCommand(IConfig& config,
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

   GetConfigValuesCommand::~GetConfigValuesCommand()
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

   void GetConfigValuesCommand::addArgument(std::string name, std::string value)
   {
      NameValuePair* configValue = m_configValuePool.allocate();

      if (configValue != NULL)
      {
         configValue->setName(value);
         configValue->setValue(string(""));

         Command::addArgument(configValue);
      }
      else
      {
         this->getLogDriver()->Log("Unable to allocate config value from pool.", LoggingError, "");
      }
   }

   CommandCompletionEvent* GetConfigValuesCommand::action()
   {
      // clear out any previous data.
      m_cmdCompletionEvent.clear();
      m_cmdCompletionEvent.setCommandID(m_commandID);
      // take first one off end of list,
      // they are added at head so this acts like a FIFO
      NameValuePair* configValue = (NameValuePair*)m_argumentList.removeFromEnd();
      while (NULL != configValue)
      {
         // Get the config value out of the configuration using the name specified
         // in the configuration parameters (expected to be previously set)
         string value = m_configuration.getValueByName(configValue->getName());

         if (value.empty())
         {
            string logStr("no value retrieved for name ");
            logStr.append(configValue->getName());
            this->getLogDriver()->Log(logStr, LoggingDebug, "");
         }
         else
         {
            m_cmdCompletionEvent.addOutputParameter(configValue->getName(), value);
         }

         // take next one off end of list
         configValue = (NameValuePair*)m_argumentList.removeFromEnd();

      }

      return &m_cmdCompletionEvent;
   }
}
