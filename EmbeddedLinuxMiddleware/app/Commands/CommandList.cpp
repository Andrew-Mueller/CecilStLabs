#include <iostream>
#include <sstream>
#include <string>
using namespace std;

#include "../../common/tersoTypes.h"               //lint !e761
#include "../../common/util/EnumByName.h"
#include "../../common/DateTime/DateTime.h"
#include "../../common/DateTime/IClockDriver.h"
#include "../../common/util/TersoUtil.h"
#include "../../common/guid.h"

#include "../../common/LinkedList/SingleLink.h"
#include "../../common/LinkedList/SingleLinkedList.h"
#include "../../common/RingBuffer/RingBuffer.h"    //lint !e28
#include "../../common/MemoryPool/MemoryPool.h"

#include "../../common/tinyxml/tinyxml2.h"
using namespace tinyxml2;

#include "../../common/logging/LogEntry.h"
#include "../../common/logging/ILogDriver.h"
#include "../../common/logging/emptyLogDriver.h"
#include "../../common/logging/ILoggable.h"

#include "../../common/config/IConfig.h"
#include "../../common/config/NullConfig.h"

#include "../../common/util/NameValuePair.h"
#include "../../common/InternalEvent/IInternalEvent.h"
#include "../../common/InternalEvent/IInternalEventHandler.h"
#include "../../common/InternalEvent/InternalEventRegistry.h"

#include "../jetstreamMessages.h"
#include "../Events/jetstreamEvent.h"
#include "../Events/CommandCompletionEvent.h"

#include "Command.h"
#include "ResetCommand.h"
#include "GetConfigValuesCommand.h"
#include "SetConfigValuesCommand.h"
#include "JetstreamCommands.h"
#include "CommandList.h"


namespace Terso
{
   CommandList::CommandList(IConfig& config,
                            IClockDriver* clockDriver,
                            std::string deviceAccessKey,
                            std::string deviceSerialNumber)
      : m_commandList(m_commandBufferAddys, sizeof(uint32_t), COMMAND_BUFFER_SIZE),
        m_jetstreamCmds(config, clockDriver, deviceAccessKey, deviceSerialNumber),
        m_commQueue(NULL)
   {
      memset(m_commandBufferAddys, 0, COMMAND_BUFFER_SIZE);
   }

   CommandList::~CommandList()
   {
      // temporary buffer
      Command* cmd[COMMAND_BUFFER_SIZE];

      // empty the ring buffer.
      // TODO: Is this unnecessary since everything is statically allocated.
      m_commandList.get(cmd, COMMAND_BUFFER_SIZE);
   }

   Command* CommandList::get()
   {
      Command* nextCommand = NULL;

      if (!m_commandList.isEmpty())
      {
         uint32_t cmdAddy = 0;

         int num_retreived = m_commandList.get(&cmdAddy, 1);

         if (1 == num_retreived)
         {
            nextCommand = (Command*)cmdAddy;
         }
      }

      return nextCommand;
   }

   void CommandList::parse(string commands_str)
   {
      // NOTE: the xml parsed here to get at the commands is different than the
      //       application side documentation.  In the future we will hopefully
      //       have device side documentation.  Until the device side
      //       documentation is complete, it is easiest to dig through
      //       the Kiosk application code for examples.

      // The received commands DO NOT need to be saved in non-volatile memory to be
      // preserved in the case of a reset.  If the device resets, we assume it
      // is because the user intended for a reset
      //
      // TODO: I hate this huge nested code.  This needs to be cleaned up.
      //       Ideally we would utilize XPath search directly and efficiently
      //       find the CommandList element required.

      XMLDocument xmlDoc;
      int success = xmlDoc.Parse(commands_str.c_str());

      if (XML_NO_ERROR == success)
      {
         // create a new Command for each entry in the list.
         XMLElement* childroot = NULL;
         XMLElement* child = NULL;

         // the root element should be "Jetstream"
         childroot = xmlDoc.RootElement();

         if ((NULL != childroot) && !childroot->NoChildren())
         {
            child = childroot->FirstChildElement("GetCommandsResponse");

            if ((NULL != child) && !child->NoChildren())
            {
               child = child->FirstChildElement("CommandList");

               if ((NULL != child) && !child->NoChildren())
               {
                  child = child->FirstChildElement("Command");

                  // iterate over all of the commands in the list.
                  while (NULL != child)
                  {
                     // TODO: get the parameter list element
                     //       (currently unused for the commands implemented)

                     string commandName = child->Attribute("Name");
                     string commandId = child->Attribute("CommandId");
                     
                     // TODO: get the ParameterList (contrary to the documentation
                     //       website, this is inside of the "Name" parameter instead
                     //       of the "CommandName" parameter).  Maybe just the difference between
                     //       the application side documentation and device
                     //       side implementation?

                     parseByCommand(child, commandName, commandId);

                     child = child->NextSiblingElement();
                  }
               }
            }
            else
            {
               // single command
               string commandName = childroot->Attribute("Name");
               string commandId = childroot->Attribute("CommandId");
               
               parseByCommand(childroot, commandName, commandId);
               
            }
         }
      }
   }
   
   void CommandList::setCommunicationQueue(CommQueue& commQueue)
   {
      m_commQueue = &commQueue;

      m_jetstreamCmds.setCommunicationQueue(&commQueue);
   }

   void CommandList::parseByCommand(XMLElement* commandElement,
                                    const std::string commandName_str,
                                    const std::string commandId)
   {
      Command* cmd = NULL;

      // TODO: this is a problem, since if two of the same commands come in, it
      //       will stomp on the previous commands values.  not sure if this is
      //       a "logical" problem or not.
      if (STR_COMPARE_MATCH == commandName_str.compare(m_jetstreamCmds.getResetCommand()->getCommandName()))
      {
         cmd = m_jetstreamCmds.getResetCommand();
      }
      else if (STR_COMPARE_MATCH == commandName_str.compare(m_jetstreamCmds.getGetConfigValuesCommand()->getCommandName()))
      {
         cmd = m_jetstreamCmds.getGetConfigValuesCommand();
      }
      else if (STR_COMPARE_MATCH == commandName_str.compare(m_jetstreamCmds.getSetConfigValuesCommand()->getCommandName()))
      {
         cmd = m_jetstreamCmds.getSetConfigValuesCommand();
      }

      // parse the arguments if there are any.
      if (NULL != commandElement)
      {
         XMLElement* child = commandElement->FirstChildElement("ParameterList");

         if (NULL != child)
         {
            for (child = child->FirstChildElement("Parameter");
                 child != NULL;
                 child = child->NextSiblingElement())
            {
               if (NULL != cmd)
               {
                  cmd->addArgument(std::string(child->Attribute("Name")),
                                   std::string(child->Attribute("Value")));
               }
            }
         }
      }

      if (cmd != NULL)
      {
         // parse the command ID into a guid and set it to the command.
         GUID cmdId(commandId);
         cmd->setCommandId(cmdId);

         // TODO: I don't know if the following need to be set for the command?
         // TODO: set the clock driver
         // TODO: set the device access key
         // TODO: set the device serial number
         // TODO: set the CommQueue

         uint32_t cmdAddy = (uint32_t)cmd;

         // add the command to the list.
         m_commandList.put(&cmdAddy, 1);
      }
   }
}
