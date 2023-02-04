#ifndef COMMAND_H
#define COMMAND_H

namespace CecilStLabs
{
   /**
    * Abstract base object for creating a command object.  A command consists of
    * an action to perform and the arguments that it requires.
    */
   class Command : SingleLink
   {
      public:
         /**
          * The action that the command needs to perform.
          *
          * @return the CommandCompletionEvent as a result of completing the
          *         commands action.
          */
         virtual CommandCompletionEvent* action() = 0;

         /**
          * Return the string with the name of the command.
          *
          * @return String containing the name of the command.
          */
         virtual std::string getCommandName() = 0;

         /**
          * add the specified argument to the list of the commands arguments.
          *
          * @param argument Pointer to a concrete argument
          *                 (memory must be managed somewhere else).
          */
         void addArgument(NameValuePair* argument);

         /**
          * Add the specified argument based on a name and a value.
          *
          * @param name The name of the argument to add.
          * @param value The value associated with the named parameter to add.
          */
         virtual void addArgument(std::string name, std::string value) = 0;

         /**
          * Get the list of arguments associated with the command.
          *
          * @return the linked list of arguments passed to this command
          */
         SingleLinkedList* getArgumentList() { return &m_argumentList; };

         /**
          * Setter for the command id for this command.
          */
         inline void setCommandId(GUID commandId) { m_commandID = commandId; };

         /**
          * Getter for the command id
          */
         inline GUID getCommandId() { return m_commandID; };

         /**
          * Gets the command completion event associated with the command.
          *
          * @return gets the command completion event.
          */
         inline CommandCompletionEvent* getCommandCompletionEvent() { return &m_cmdCompletionEvent; };

         /**
          * Sets the event handler to use to notify, if needed that a setting is available
          * 
          * @param event The event object
          */
         inline void setEventHandler( InternalEventRegistry* event ) { m_event = event; }

      protected:

         /**
          * Protected constructor to allow child classes to properly initialize
          * it.
          * @param deviceAccessKey The Jetstream Access Key.
          * @param deviceSerialNumber The Jetstream Serial Number.
          * @param clockDriver The driver for accessing the clock for the system.
          * @param commQueue Communication queue to allow the command to
          *                  re-enqueue responses.
          * @param commandID ID of this command.
          */
         Command(std::string deviceAccessKey,
                 std::string deviceSerialNumber,
                 IClockDriver* clockDriver)
            : m_cmdCompletionEvent(deviceAccessKey,
                                   deviceSerialNumber,
                                   clockDriver),
              m_event(NULL)
         {
            // intentionally left blank.
         };

         // define the constructor, copy constructor, destructor and assignment
         // operator to ensure the abstract class cannot be instantiated.
         Command() : SingleLink() {};
         Command(Command& cmd __attribute__((unused))) : SingleLink() {};
         virtual ~Command() {};
         Command& operator=(const Command& rhs __attribute__((unused))) { return *this; };

         /**
          * The id of the command queued by Jetstream and executed by the device.
          * This is required to send back with the command completion event after
          * the command has been executed.
          */
         GUID m_commandID;

         /**
          * List of arguments required for executing the command.
          */
         SingleLinkedList m_argumentList;

         /**
          * The result of the action of the command.
          */
         CommandCompletionEvent m_cmdCompletionEvent;

         /**
          * Removes all elements from the list of arguments.
          */
         void clearArgumentList();
         
         /**
          * internal event notification object to notify, if needed, that a new setting is available
          */
         InternalEventRegistry* m_event;

   };
}

#endif
