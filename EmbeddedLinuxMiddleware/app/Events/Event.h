#ifndef EVENT_H_
#define EVENT_H_

namespace CecilStLabs
{

   // forward declare the commqueue to work with the required circular reference
   // (specifically including Event.h from CommQueue.cpp)
   class CommQueue;

   /**
    * Abstract base class for events that are posted to the server.  Encapsulate
    * some of the common items needed to post data such as the clock driver to
    * get the current time of the event, the HTTPS driver, the url to post to,
    * the device access key GUID, and the logical device id (serial number).
    */
   class Event
   {
      public:

         /**
          * Post the event to the device end point.
          */
         void Post();

         /**
          * retrieves the event xml to post to the server.
          *
          * @return string representation of the event XML.
          */
         std::string getXML() { return buildXML(); };

      protected:

         /**
          * Unused default constructor.  Declare it here to prevent the compiler
          * from creating it automatically.s
          * NOTE: the constructor is protected to prevent the class from being
          *       able to be created.
          */
         Event();

         /**
          * create the event object with the parts needed
          * to properly post it to the server.
          * NOTE: the constructor is protected to prevent the class from being
          *       able to be created.
          */
         Event(std::string deviceAccessKey,
               std::string deviceSerialNumber,
               IClockDriver* clockDriver,
               eMessage jsMessageID);

         /**
          * release resources used by the jetstream event.
          */
         virtual ~jetstreamEvent();

         /**
          * Setter for the communication queue object for enqueing event communications.
          *
          * @param commQueue Pointer to the communications queue object to use.
          */
         void setCommunicationQueue(CommQueue* commQueue);

         /**
          * The logical device id  assigned to this device.
          */
         std::string m_logicalDeviceID;

         /**
          * Device time that the event occurred.
          */
         DateTime m_eventTime;

         /**
          * Interface for to abstract the hardware specific clock driver.
          */
         IClockDriver* m_clockDriver;

         /**
          * Communications data access layer for enqueuing messages.
          */
         CommQueue* m_commQueue;

         /**
          * URL to the Jetstream end point for posting the event.
          */
         std::string m_eventUrl;

         /**
          * Identifier of the Jetstream message.
          */
         eJetstreamMessage m_jsMessage;

         /**
          * the Jetstream Device Access Key
          */
         std::string m_deviceAccessKey;

         /**
          * Build the XML that represents the event in the object that is a child
          * of this jetstreamEvent object.
          */
         virtual std::string buildXML() = 0;
   };

}
#endif
