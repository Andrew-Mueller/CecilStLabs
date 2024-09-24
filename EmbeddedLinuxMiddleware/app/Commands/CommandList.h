#ifndef COMMAND_LIST_H
#define COMMAND_LIST_H

#include "../../common/config/NullConfig.h"

#include "../../common/tinyxml/tinyxml2.h"
using namespace tinyxml2;

namespace CecilStLabs
{
   // forward declare the communication queue item.
   class CommQueue;

   /**
    * Container of commands received from the API Server.
    */
   class CommandList
   {
      public:

         /**
          * Create the empty list of commands.
          */
         CommandList(IConfig& wholeConfig,
                     IClockDriver* clockDriver,
                     std::string deviceAccessKey,
                     std::string deviceSerialNumber);

         /**
          * Release all resources used by this command list.
          */
         virtual ~CommandList();

         /**
          * Remove the next command from the list and returns it.
          *
          * @return The command in the list to retrieve.
          */
         Command* get();

         /**
          * Parse the commands in the Command List specified in the string argument.
          *
          * @param commands_str The string representation of the command to parse.
          */
         void parse(std::string commands_str);

         /**
          * Setter for the communication Queue object to allow parsed messages
          * to respond to the API Server.
          *
          * @param commQueue Communication Queue reference.
          */
         virtual void setCommunicationQueue(CommQueue& commQueue);

      protected:

         /**
          * The number of the commands to buffer waiting for the connection to
          * return.
          */
         static const uint8_t COMMAND_BUFFER_SIZE = 100;

         // NOTE: consider moving these constants to basicTypes.h
         static const int8_t STR_COMPARE_MATCH = 0;
         static const int8_t STR_COMPARE_SHORTER_OR_LESS = -1;
         static const int8_t STR_COMPARE_LONGER_OR_GREATER = 1;

         /**
          * The concrete buffer to contain the pointer to commands.
          */
         uint32_t m_commandBufferAddys[COMMAND_BUFFER_SIZE];

         /**
          * The ordered list of commands to execute.
          */
         RingBuffer m_commandList;

         /**
          * The concrete instances of the commands.
          */
         AppCommands m_appCmds;

         /**
          * pointer to the communication queue for enqueuing command responses.
          */
         CommQueue* m_commQueue;

         /**
          * Parse the command based on the name
          *
          * @param commandElement The XML containing the command to parse.
          * @param commandName_str The name of the command to parse.
          * @param commandId The unique id of the command to parse.
          */
         virtual void parseByCommand(tinyxml2::XMLElement* commandElement,
                                     const std::string commandName_str,
                                     const std::string commandId);

         // define the copy constructor and the assignment operator to ensure
         // they are not automatically created by the compiler.
         // TODO: the copy constructor causes errors down the chain, so its commented out here for now... this sucks, it sucks, sucks
         //CommandList(CommandList& copy __attribute__((unused))) : m_commandList(NULL, 0, 0), m_appCmds(copy.m_appCmds) { };
         CommandList& operator=(CommandList& rhs __attribute__((unused))) {return rhs; };
   };
}
#endif
