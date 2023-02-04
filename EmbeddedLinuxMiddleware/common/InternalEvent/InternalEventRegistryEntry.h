#ifndef INTERNALEVENTREGISTRYENTRY_H_
#define INTERNALEVENTREGISTRYENTRY_H_

namespace CecilStLabs
{   
    
   /**
    * TODO: update the documentation for all of these events. 
    */
   class InternalEventRegistryEntry : public SingleLink
   {
      public:
         InternalEventRegistryEntry( eInternalEventType type, IInternalEventHandler* handler );
         virtual ~InternalEventRegistryEntry();

         inline eInternalEventType getEventType() { return m_eventType; }
         inline IInternalEventHandler* getEventHandler() { return m_eventHandler; }
         
      protected:
         
         /**
          * event type of this instance 
          */
         eInternalEventType m_eventType;
         
         /**
          * event handler object
          */
         IInternalEventHandler* m_eventHandler; 

   };
}
#endif
