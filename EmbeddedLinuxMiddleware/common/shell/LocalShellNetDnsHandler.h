// TODO: this whole LocalShell system is in desperate need of cleanup and refactoring.

#ifndef LOCALSHELLNETDNSHANDLER_H_
#define LOCALSHELLNETDNSHANDLER_H_

namespace CecilStLabs
{

   /**
    * Shell handler to process DNS commands
    */
   class LocalShellNetDnsHandler : public LocalShellBaseHandler
   {
      public:
         /**
          * Create the DNS shell command handler
          *
          * @param config the config section for the network parameters
          */
         LocalShellNetDnsHandler( IConfig& config );

         /**
          * release the resources used by the DNS shell handler.
          */
         virtual ~LocalShellNetDnsHandler();


         /**
          * process the command that the user entered.
          *
          * @param command string that the user entered
          * @param delim   the delimiter between parts of command
          * @param action  The specific command action to perform
          *
          * @return
          *
          */
         virtual eShellHandlers processCommand( const string& command, const char delim, ICommandAction* action );

      protected:
         /**
          * process the ? command.
          */
         virtual void cmdHelp();

         /**
          * process the command to show the current dns addresses
          *
          * @param command string that the user entered
          * @param action  the type of action like a system call
          *
          * @return true if command processed, false if not
          *
          */
         void cmdShow( const string& command, ICommandAction* action );

         /**
          * process the command to remove a dns address
          *
          * @param command string that the user entered
          * @param action  the type of action like a system call
          *
          * @return true if command processed, false if not
          *
          */
         void cmdRemove( const string& command, ICommandAction* action );

         /**
          * process the command to add a dns address
          *
          * @param command string that the user entered
          * @param action  the type of action like a system call
          *
          * @return true if command processed, false if not
          *
          */
         void cmdAdd( const string& command, ICommandAction* action );

         /**
          * constants to represent the command strings
          */
         static const string SHELL_DNS_PROMPT;
         static const string SHELL_DNS_SHOW;
         static const string SHELL_DNS_REMOVE;
         static const string SHELL_DNS_ADD;
         static const string SHELL_DNS_BACK;
   };
}
#endif
