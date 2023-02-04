#ifndef SETCONFIGVALUESCOMMAND_H_
#define SETCONFIGVALUESCOMMAND_H_

namespace Terso
{
   /**
    * Jetstream command for setting the configuration values to a new value in
    * the system.
    */
   class SetConfigValuesCommand : public Command, public ILoggable
   {
      public:

         /**
          * Create the jetstream set configuration values command.
          */
         SetConfigValuesCommand(IConfig& config,
                                IClockDriver* clockDriver,
                                std::string deviceAccessKey,
                                std::string deviceSerialNumber);

         /**
          * Release all of the resources used by the command.
          */
         virtual ~SetConfigValuesCommand();

         /**
          * Return the string name of the SetConfigValuesCommand.
          *
          * @return The Name of the Get Config Values Command.
          */
         std::string getCommandName() { return "SetConfigValuesCommand"; };

         /**
          * Add the specified configuration parameter with the name and value.
          *
          * @param name Name the configuration parameter to add.
          * @param value Parameter value to add.
          */
         void addArgument(std::string name, std::string value);

         /**
          * Sets the config value specified.
          *
          * @@return the CommandCompletionEvent as a result of completing the
          *          Set Config Values commands action (expected to be empty).
          */
         CommandCompletionEvent* action();
         
         /**
          * Setter for the communication Queue object to allow parsed messages
          * to respond to
          *
          * @param commQueue Communication Queue reference.
          */
         inline void setCommunicationQueue(CommQueue* commQueue) { m_cmdCompletionEvent.setCommunicationQueue(commQueue); };

      protected:

         /**
          * The maximum number of parameters currently available for the
          * RFID Enclosures
          */
         static const uint8_t MAX_NUM_PARAMETERS = 27;

         /**
          * concrete memory allocation for the maximum number of
          * parameters available to get or set.
          */
         NameValuePair m_configValues[MAX_NUM_PARAMETERS];

         /**
          * Memory pool for managing the configuration values.
          */
         MemoryPool<NameValuePair> m_configValuePool;

         /**
          * The configuration member interface to set config values to.
          */
         IConfig& m_configuration;
         
   };

}
#endif
