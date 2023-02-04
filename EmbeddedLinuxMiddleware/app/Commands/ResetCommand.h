#ifndef RESETCOMMAND_H
#define RESETCOMMAND_H

namespace Terso
{
   /**
    * Jetstream command to reset the device.
    *
    * TODO: does the reset command require a command completion event?
    *       documentation says yes:
    *       https://jetstreamrfid.com/Documentation/CommandCompletionEvent
    */
   class ResetCommand : public Command
   {
      public:

         /**
          * Create the reset command object.
          */
         ResetCommand();

         /**
          * Release the command's resources.
          */
         virtual ~ResetCommand();

         /**
          * Reset the system
          *
          * @return All commands need to return a command completion event, but
          *         reset... since it resets does not return a command completion
          *         event.
          */
         CommandCompletionEvent* action();

         /**
          * Return the string name of the ResetCommand.
          *
          * @return The Name of the Reset Command.
          */
         std::string getCommandName() { return "ResetCommand"; };

         /**
          * Implement inherited method for adding arguments.
          *
          * No arguments are expected for the reset command.
          * just ignore them.
          *
          * @param name Name of the parameter to add.
          * @param value Value of the parameter to add.
          */
         void addArgument( std::string name __attribute__((unused)), std::string value __attribute__((unused))) {};

         // TODO: signal(SIGTERM, Cleanup); can be used to call a Cleanup
         //       function when the Terminate signal is sent.

      protected:

         /**
          * Constant command defined by Linus for restarting the system.
          */
         static const int LINUX_REBOOT_CMD_RESTART = 0x1234567;
   };
}

#endif
