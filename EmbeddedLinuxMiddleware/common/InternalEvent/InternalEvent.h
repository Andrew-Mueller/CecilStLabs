#ifndef INTERNALEVENT_H_
#define INTERNALEVENT_H_


namespace CecilStLabs
{   
   typedef struct 
   {
      int        size;                  // total size for the EVENT_DATA
      eEventType type;
      char       data[MAX_EVENT_DATA+1];  // place holder space for data - maybe not all used or sent 
   } EVENT_DATA;

   /**
    * API to handle internal events
    */   
   class InternalEvent
   {
      public:
         InternalEvent();
         virtual ~InternalEvent();

         /**
          * Send an event with data to another part of program
          * 
          * @param type  type of event
          * @param data  extra data to send with event (optional can be null)
          * @param size  size of the data to send (0 if no data)
          * 
          * @return true if successful
          */
         virtual bool SendEvent( const eEventType type, const char* data, const int size );  
         
         /**
          * Wait for the event in this instance to be triggered
          * 
          * @param data     data returned with event
          * @param size     input- max size of data buffer sent in
          *                 output- size of the data retrieved (includes NULL termination)
          * @param timeout  timeout in milliseconds (-1 for infinite)
          * 
          * @return type of event returned
          * 
          */
         eEventType WaitOnEvent( char* data, int* size, const int timeout );  
         
      protected:
         
         /**
          * Read events from pipe and eithe return or queue
          * 
          * @param data    data returned with event
          * @param value   size of the data retrieved
          * 
          * @return type of event returned
          * 
          */
         eEventType ReadFromPipe( char* data, int* size ); 
         
        /**
          * file descriptors used by the pipe of this event (2 needed - read and write);
          */
         int m_fd[2];
         
        /**
          * place to store max fd value for use in select
          */
         int m_maxfd;
         
         /**
          * list of events already received - 
          */
         SingleLinkedList m_Events;
        
         CecilStLabs::CriticalSection m_criticalSection;

   };
}
#endif
