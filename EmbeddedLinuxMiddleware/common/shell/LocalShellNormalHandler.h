// TODO: this whole LocalShell system is in desperate need of cleanup and refactoring.

#ifndef LOCALSHELLNORMALHANDLER_H_
#define LOCALSHELLNORMALHANDLER_H_

namespace CecilStLabs
{
   class LocalShellNormalHandler : public LocalShellBaseHandler
   {
      public:

         LocalShellNormalHandler( IConfig& config );

         /**
          * Release the resources used for this handler
          */
         virtual ~LocalShellNormalHandler();


         /**
          * process the command that the user entered.
          *
          * @param command string that the user entered
          * @param delim   the delimiter between parts of command
          * @param action  the type of action like a system call
          *
          * @return Shell Handler enumeration as a result of processing the command.
          *
          */
         virtual eShellHandlers processCommand( const std::string& command, const char delim, ICommandAction* action );

      protected:
         
         // override this to add help for commands in derived class
         virtual void cmdHelp();
   };
}
#endif
