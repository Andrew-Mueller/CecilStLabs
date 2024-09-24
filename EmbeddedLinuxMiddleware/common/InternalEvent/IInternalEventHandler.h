#ifndef IINTERNALEVENTHANDLER_H_
#define IINTERNALEVENTHANDLER_H_

namespace CecilStLabs
{   
    
   /**
    * Object interface to process internal events
    */   
   class IInternalEventHandler
   {
      public:

         /**
          * Process the specified event.
          * 
          * @param event Event to process
          * @return true 
          */
         virtual bool processEvent( IEVENT_DATA& event ) = 0;
         
      protected:
         
         /**
          * NOTE: the constructor is protected to ensure that this abstract
          *       object cannot be created.
          */
         IInternalEventHandler() {};

         /**
          * NOTE: the destructor is protected to ensure that it cannot be called
          *       (since it cannot be created).
          */
         
         virtual ~IInternalEventHandler() {};
         
                  
   };
}
#endif
