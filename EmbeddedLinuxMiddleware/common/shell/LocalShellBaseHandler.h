// TODO: this whole LocalShell system is in desperate need of cleanup and refactoring.

#ifndef LOCALSHELLBASEHANDLER_H_
#define LOCALSHELLBASEHANDLER_H_

namespace CecilStLabs
{

   /**
    * Enumerated shell command handlers
    * TODO: this does not belong here as it makes this base handler non-abstract
    */
   enum eShellHandlers
   {
      HANDLER_INVALID  = 0,
      HANDLER_EXIT     = 1,
      HANDLER_NORMAL   = 2,
      HANDLER_NET      = 3,
      HANDLER_NET_DNS  = 4
   };

   /**
    * Local shell state functionality to process commands
    */
   class LocalShellBaseHandler
   {
      public:
         /**
          * constructor for class
          *
          * @param config network configuration section
          */
         LocalShellBaseHandler( IConfig& config );
         virtual ~LocalShellBaseHandler();

         /**
          * process the command that the user entered.
          *
          * @param command string that the user entered
          * @param delim   the delimiter between parts of command
          * @param action  the type of action like a system call
          *
          * @return true if command processed, false if not
          *
          */
         virtual eShellHandlers processCommand( const string& command, const char delim, ICommandAction* action );

         inline eShellHandlers getType() { return m_type; }
         inline string getPrompt() { return m_prompt; }

         // TODO: this should not be here.
          /**
          * Splits a string with the specified delimeter and puts parts in elems
          *
          * @param str string to split
          * @param delim delimiter to look for
          * @param elems array of strings to put the parts
          *
          */
          void split(const string& str, const char delim, vector<string> &elems);

      protected:

         /**
          * process the command  '?'
          */
         virtual void cmdHelp();

         /**
          * process the command 'license'
          */
         bool cmdLicense();


         /**
          * config object with network info
          */
         IConfig& m_configuration;

         /**
          * the prompt that will show by command entry
          */
         string m_prompt;

         /**
          * type of handler set by the constructor
          */
         eShellHandlers m_type;
   };
}
#endif
