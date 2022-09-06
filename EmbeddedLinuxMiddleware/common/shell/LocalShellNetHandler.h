// TODO: this whole LocalShell system is in desperate need of cleanup and refactoring.

#ifndef LOCALSHELLNETHANDLER_H_
#define LOCALSHELLNETHANDLER_H_

namespace CecilStLabs
{

   /**
    * Local shell state functionality to process commands
    */
   class LocalShellNetHandler : public LocalShellBaseHandler
   {
      public:
         LocalShellNetHandler( IConfig& config );
         virtual ~LocalShellNetHandler();


         /**
          * process the command that the user entered.
          *
          * @param command string that the user entered
          * @param delim   the delimiter between parts of command
          * @param action  the type of action like a system call
          *
          * @return Shell Handler Enumeration as a result of processing the command.
          *
          */
         virtual eShellHandlers processCommand (const std::string& command, const char delim, ICommandAction* action);

      protected:

         virtual void cmdHelp();

         /**
          * process the IP command to set or get the IP address
          *
          * @param command string that the user entered
          * @param delim   the delimiter between parts of command
          * @param action  the type of action like a system call
          *
          */
         void cmdIP (const std::string& command, const char delim, ICommandAction* action);

         /**
          * process the subnet command to set or get the subnet address
          *
          * @param command string that the user entered
          * @param delim   the delimiter between parts of command
          * @param action  the type of action like a system call
          *
          */
         void cmdSubnet (const std::string& command, const char delim, ICommandAction* action);

         /**
          * process the gateway command to set or get the gateway address
          *
          * @param command string that the user entered
          * @param delim   the delimiter between parts of command
          * @param action  the type of action like a system call
          *
          */
         void cmdGateway (const std::string& command, const char delim, ICommandAction* action);

          /**
          * process the url command to set or get the url address
          *
          * @param command string that the user entered
          * @param delim   the delimiter between parts of command
          * @param action  the type of action like a system call
          *
          */
        void cmdURL (const std::string& command, const char delim, ICommandAction* action);

         /**
          * process the accessKey command to set or get the access key string
          *
          * @param command string that the user entered
          * @param delim   the delimiter between parts of command
          * @param action  the type of action like a system call
          *
          */
         void cmdAccessKey (const std::string& command, const char delim, ICommandAction* action);

         /**
          * process the deviceID command to set or get the device ID string
          *
          * @param command string that the user entered
          * @param delim   the delimiter between parts of command
          * @param action  the type of action like a system call
          *
          */
         void cmdDeviceId (const std::string& command, const char delim, ICommandAction* action);

          /**
          * Checks if an IP address is formatted properly
          *
          * @param ipAddress ip address string
          *
          * @return true if address is valid, false if not
          *
          */
         bool isValidIpAddress (const std::string& ipAddress);


   };
}
#endif
