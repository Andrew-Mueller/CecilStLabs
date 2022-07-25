// TODO: this whole LocalShell system is in desperate need of cleanup and refactoring.
//      It appears that the engineer who first implemented the shell was in a hurry, or inexperienced, etc.
//      - tons of incorrect and inaccurate copy pasta code
//      - naming needs some TLC.
//      - lots of functions, types, behaviors slammed into classes into
//      - mixing a lot of C style functions and pound defines instead of constants
//      - incorrect documentation (copy pasta) and poorly named functions and variables.
//      - PAM ifdef'd in.
//      - duplicate constant definitions
//      - unrelated utility function included in classes.
//
#ifndef LOCALSHELL_H_
#define LOCALSHELL_H_

namespace CecilStLabs
{
   enum eAuthCode
   {
      AUTH_VALID   = 0,
      AUTH_INVALID = 1
   };

   /**
    * Local shell functionality to send commands to unit
    */
   class LocalShell
   {
      public:
         LocalShell( IConfig& config );
         virtual ~LocalShell();

         /**
          * Execute the intro part before shell is enter. The Into part will prompt
          * the user to enter 's' to enter the shell. There is a timeout value that if the
          * 's' is not pressed, it will exit the shell program
          *
          * @return true if shell should be executed
          */
         bool executeIntro();

        /**
          * Execute the main shell command processing.
          */
         void executeShell();

         /**
          * Authenticate the user to ensure the specified user has acceses to the shell.
          *
          * @param user user name
          *
          * @return eAuthCode return value indicating if the user was authenticated
          */
         eAuthCode authenticate( char* user );

         /**
          * process the handler with the command that the user entered.
          *
          * @param command string that the user entered
          * @param action  the type of action like a system call
          *
          * @return true if command processed, false if not
          *
          */
         virtual eShellHandlers processHandler( const string& command, ICommandAction* action );

         /**
          * Getter for the configuration
          */
         inline IConfig& getConfig() { return m_configuration; }

         // timeout in seconds that the shell will wait before exiting
         static const uint8_t SHELL_TIMEOUT_SECONDS = 5;

         static const uint16_t MAX_LENGTH = 1024;
         static const char[2] DELIMS = {'\r', '\n'};

         // local hard-coded user and password
         // TODO: these need to be removed.
         static const std::string LOCAL_USER;
         static const std::string LOCAL_PASSWORD;

      protected:

         /**
          * pointer to the current handler object
          */
         LocalShellBaseHandler* m_pHandler;

         /**
          * list of all available handlers
          */
         vector<LocalShellBaseHandler*> m_vHandlers;

         /**
          * flag to indicate the shell should be entered to kill timeout
          * TODO: this should obviously be changed to m_isTimedOut
          */
         bool m_enterShell;

         /**
          * flag set when 'exit' command entered
          */
         bool m_exit;

         /**
          * Current message handler type
          */
         eShellHandlers m_curHandler;

         /**
          * configuration object with
          * TODO: why is this needed?
          */
         IConfig& m_configuration;

         #ifdef USE_PAM
            /**
             * structure used by PAM to tie into conversation functionality
             * TODO: what the F is this?
             */
            struct pam_conv m_conversation;
         #endif
   };
}
#endif
