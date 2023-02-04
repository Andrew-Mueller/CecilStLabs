#ifndef SINGLEEVENT_H_
#define SINGLEEVENT_H_

#define MAX_EVENT_DATA 256

namespace CecilStLabs
{   
    
   enum eEventType
   {
      EVT_NONE = 0, 
      EVT_GENERIC, 
      EVT_SETCONFIG
   };
   

   /**
    * API to handle internal events
    */   
   class SingleEvent : public SingleLink
   {
      public:
         /**
          * constructor for class
         */
         SingleEvent( const eEventType type, const int size, const char* data );
         
         
         /**
          * destructor
          */
         virtual ~SingleEvent();


         /**
          * Accessors to get members
          */
         int getTotalSize() { return totalSize; }
         int getDataSize() { return dataSize; }
         eEventType getEventType() { return eventType; }
         char* getData() { return eventData; }
         
      protected:
         
         /**
          * total size of the event data + size of int + size of eEventType 
          */
         int totalSize;
         
         
         /**
          * event type of this instance 
          */
         eEventType eventType;
         
         /**
          * size of just the event data
          */
         int dataSize;

         /**
          * palce holder for data. It all may not be used. Dictated by size value
          */
         char eventData[MAX_EVENT_DATA]; 
 
   };
}
#endif
