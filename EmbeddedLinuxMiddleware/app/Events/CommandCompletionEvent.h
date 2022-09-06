#ifndef COMMANDCOMPLETIONEVENT_H_
#define COMMANDCOMPLETIONEVENT_H_

namespace CecilStLabs
{

   class CommandCompletionEvent : public Event, public ILoggable
   {
      public:

         /**
          * default constructor to simplify some of the
          */
         CommandCompletionEvent() {};

         /**
          * Create the command completion event that is used to mark the completion of
          * some of the "long polling" events initiated by the protocol.
          *
          * @param deviceAccessKey The device access key.
          * @param deviceSerialNumber The serial number
          * @param clockDriver The clock driver to get the current time to
          *                    generate the command completion event.
          * @param commandID Unique command ID
          */
         CommandCompletionEvent(std::string deviceAccessKey,
                                std::string deviceSerialNumber,
                                IClockDriver* clockDriver);

         /**
          * release all resources consumed by the command completion event.
          */
         virtual ~CommandCompletionEvent();

         /**
          * Add the specified name and value as the output parameters
          *
          * @param name The name of the parameter to add.
          * @param value The value of the parameter to add, associated with the name.
          */
         void addOutputParameter(std::string name, std::string value);

         /**
          * Gets the list of output parameters added to the command completion event.
          *
          * @return Linked list of output parameters (name/value pairs).
          */
         inline SingleLinkedList* getOutputParameterList() { return &m_parameterList; };

         /**
          * Clears the list of output parameters
          */
         void clear();

         /**
          * Setter for the command that we are completing.
          *
          * @param commandID GUID representing the command completion id.
          */
         inline void setCommandID(GUID commandID) { m_commandID = commandID; };

         /**
          * Setter for the communication queue to enqueue the completion event.
          * @param commQueue
          */
         inline void setCommunicationQueue(CommQueue* commQueue) { Event::setCommunicationQueue(commQueue); };

      protected:

         // define the copy constructor as protected so the compiler doesn't
         // create one for us.
         CommandCompletionEvent(CommandCompletionEvent& copy __attribute__((unused))) : Event((Event&)copy), ILoggable() {};

         /*
          * The number of parameter values that can be added.
          *
          * // TODO: when the Maximum number of tags goes up, this maximum number
          *          of parameters will also need to go up to match.
          */
         static const uint16_t MAX_PARAMETER_ITEMS = 1024;

         /**
          * The unique command id for the command completion event.
          */
         GUID m_commandID;

         /**
          * The concrete list of Name and Value pair items in the memory pool
          * to be added to the list.
          */
         NameValuePair m_parameters[MAX_PARAMETER_ITEMS];

         /**
          * list of parameters to be sent back with the command completion event.
          */
         SingleLinkedList m_parameterList;

         /**
          * Pool of available name value pairs.
          */
         MemoryPool<NameValuePair> m_parameterPool;

         /**
          * Build the XML that represents the event in the object that is a child
          * of this jetstreamEvent object.
          */
         std::string buildXML();
   };

}
#endif
