#ifndef INTERNALEVENTREGISTRY_H_
#define INTERNALEVENTREGISTRY_H_


namespace CecilStLabs
{   
   /**
    * API to handle internal events
    */   
   class InternalEventRegistry : public ILoggable
   {
      public:
         /**
          * constructor for class
         */
         InternalEventRegistry();
         
         
         /**
          * destructor
          */
         virtual ~InternalEventRegistry();

         /**
          * Send an event with data to another part of program
          * 
          * @param event reference to event object 
          * 
          * @return true if successful
          */
         bool sendEvent( IInternalEvent& event );  
         
         /**
          * Register to receive a specific event
          * 
          * @param type    the event type to watch for  
          * @param handler the handler to call when event is recieved  
          * 
          */
         void registerEvent( eInternalEventType type, IInternalEventHandler* handler );  
                  
         /**
          * Start the registry thread
          */
         void start();
         
         /**
          * stop the registry thread
          */
         void stop();
         
         /** 
          * returns running state of thread
          */
         bool isEventThreadRunning() { return m_eventThreadRunning; }
         
         /**
          * Wait for events to come through internal pipe, then send to registered handlers
          * This function only to be called by thread, but it need to be public to do that
          */
         void waitOnEvents();  
         
      protected:
         
         
         /**
          * Read events from pipe and either return or queue
          * 
          * @param data    data returned with event
          * 
          * @return true if an event read from pipe
          * 
          */
         bool readFromPipe( IEVENT_DATA& data ); 
         
        /**
          * file descriptors used by the pipe of this event (2 needed - read and write);
          */
         int m_fd[2];
         
        /**
          * place to store max fd value for use in select
          */
         int m_maxfd;
               
         /**
          * Flag to stop event handling thread
          */
         bool m_eventThreadRunning;

         /**
          * Set event thread id
          */
         pthread_t m_eventThread;
         
         /**
          * list of events already received - 
          */
         SingleLinkedList m_RegisteredEvents;
        
         /**
          * semaphore protection for pipe holding events.
          */
         pthread_mutex_t m_pipeSem;
         

   };
   
   /**
    * Non-member function used for starting up the thread.
    *
    * @param  arg pointer to InternalEventRegistry object
    */
   void* EventThread(void* arg);

}
#endif
