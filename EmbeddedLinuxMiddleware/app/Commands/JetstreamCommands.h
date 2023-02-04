#ifndef JETSTREAMCOMMANDS_H_
#define JETSTREAMCOMMANDS_H_

namespace Terso
{
   /**
    * concrete instances of all of the possible Jetstream Commands will be defined
    * here with the intention of having a single comprehensive object that we can
    * retrieve their instances of.
    */
   class JetstreamCommands
   {
      public:

         /**
          * Create the Jetstream Commands object containing concrete instances
          * of all possible Jetstream commands.
          */
         JetstreamCommands(IConfig& config,
                           IClockDriver* clockDriver,
                           std::string deviceAccessKey,
                           std::string deviceSerialNumber)
          : m_resetCmd(),
            m_getConfigValuesCmd(config, clockDriver, deviceAccessKey, deviceSerialNumber),
            m_setConfigValuesCmd(config, clockDriver, deviceAccessKey, deviceSerialNumber)
         {
            // intentionally left blank.
         };

         /**
          * Release all resources consumed by the concrete instances of the
          * Jetstream Commands.
          */
         virtual ~JetstreamCommands() {}

         /**
          * Getter for the reset command.
          *
          * @return The concrete instance of the Reset Command.
          */
         inline Command* getResetCommand() { return &m_resetCmd; };

         /**
          * Getter for the get config values command.
          *
          * @return The concrete instance of the Get Config Values Command.
          */
         inline Command* getGetConfigValuesCommand() { return &m_getConfigValuesCmd; };

         /**
          * Getter for the set config values command.
          *
          * @return The concrete instance of the Set Config Values Command.
          */
         inline Command* getSetConfigValuesCommand() { return &m_setConfigValuesCmd; };
         

         /**
          *
          * @param commQueue
          */
         inline void setCommunicationQueue(CommQueue* commQueue) { m_getConfigValuesCmd.setCommunicationQueue(commQueue);
                                                                   m_setConfigValuesCmd.setCommunicationQueue(commQueue); };
                                                                                                                             

      protected:

         /**
          * Concrete instance of the Reset command.
          */
         ResetCommand m_resetCmd;

         /**
          * Concrete instance of the Get Config Values Command.
          */
         GetConfigValuesCommand m_getConfigValuesCmd;

         /**
          * Concrete instance of the Set Config Values Command.
          */
         SetConfigValuesCommand m_setConfigValuesCmd;

         /**
          * Concrete instance of the get EPC List Command.
          */
         // TODO: need to add commands after they have been implemented.
         //GetEPCListCommand m_getEPCListCmd;

         /**
          * Concrete instance of the Firmware Update Command.
          */
         // TODO: need to add commands after they have been implemented.
         //UpdateFirmwareCommand m_updateFirmwareCmd;
   };
}

#endif
