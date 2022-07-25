#ifndef GENERICINTERNALEVENT_H_
#define GENERICINTERNALEVENT_H_

namespace CecilStLabs
{   
   /**
    * Generic (no data) event type
    */   
   class GenericInternalEvent : public IInternalEvent
   {
      public:
         GenericInternalEvent();
         virtual ~GenericInternalEvent();    
   };
}
#endif
