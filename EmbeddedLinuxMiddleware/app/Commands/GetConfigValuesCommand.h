#ifndef GETCONFIGVALUESCOMMAND_H_
#define GETCONFIGVALUESCOMMAND_H_

namespace Terso
{
   /**
    * Jetstream command for getting the configuration values currently set in
    * the system.
    */
   class GetConfigValuesCommand : public Command, public ILoggable
   {
      public:
         /**
          * Create the get configuration values command.
          *
          * @param config the Coniguration interface used to retrieve values.
          */
         GetConfigValuesCommand(IConfig& config,
                                IClockDriver* clockDriver,
                                std::string deviceAccessKey,
                                std::string deviceSerialNumber);

         /**
          * Release all of the resources used by the command.
          */
         virtual ~GetConfigValuesCommand();

         /**
          * Return the string name of the GetConfigValuesCommand.
          *
          * @return The Name of the Get Config Values Command.
          */
         std::string getCommandName() { return "GetConfigValuesCommand"; };

         /**
          * Add the specified configuration parameter with the name and value.
          *
          * @param name Name the configuration parameter to add.
          * @param value Parameter value to add.
          */
         void addArgument(std::string name, std::string value);

         /**
          * Retrieves the config value requested.
          *
          * @return the CommandCompletionEvent as a result of completing the
          *         commands action.
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
          * The maximum number of parameters that can be set at once.
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
          * The configuration member interface to get config values from.
          */
         IConfig& m_configuration;


   };

}
#endif
