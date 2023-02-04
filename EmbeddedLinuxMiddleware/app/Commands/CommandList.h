#ifndef COMMAND_LIST_H
#define COMMAND_LIST_H

#include "../../common/config/NullConfig.h"

#include "../../common/tinyxml/tinyxml2.h"
using namespace tinyxml2;

namespace Terso
{
   // forward declare the communication queue item.
   class CommQueue;

   /**
    * Container of commands received from the Jetstream Server.
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
          * @param The command in the list to retrieve.
          *
          * @return True if the command was retrieved from the list successfully,
          *         False if it was not (not in the list, list is empty, etc...).
          */
         Command* get();

         /**
          * Parse the commands in the Command List specified in the
          */
         void parse(std::string commands_str);

         /**
          * Setter for the communication Queue object to allow parsed messages
          * to respond to Jetstream.
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

         // NOTE: consider moving these constants to tersoTypes.h
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
          * The concrete instances of the jetstream commands.
          */
         JetstreamCommands m_jetstreamCmds;

         /**
          * pointer to the communication queue for enqueuing command responses.
          */
         CommQueue* m_commQueue;

         /**
          * Parse the command based on the name
          *
          * @param commandName_str The name of the command to parse.
          */
         virtual void parseByCommand(tinyxml2::XMLElement* commandElement,
                                     const std::string commandName_str,
                                     const std::string commandId);

         // define the copy constructor and the assignment operator to ensure
         // they are not automatically created by the compiler.
         // TODO: the copy constructor causes errors down the chain, so its commented out here for now... this sucks, it sucks, sucks
         //CommandList(CommandList& copy __attribute__((unused))) : m_commandList(NULL, 0, 0), m_jetstreamCmds(copy.m_jetstreamCmds) { };
         CommandList& operator=(CommandList& rhs __attribute__((unused))) {return rhs; };
   };
}
#endif
