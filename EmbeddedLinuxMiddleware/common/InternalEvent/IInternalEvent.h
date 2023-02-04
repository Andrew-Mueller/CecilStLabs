#ifndef IINTERNALEVENT_H_
#define IINTERNALEVENT_H_

#define MAX_EVENT_DATA 256

namespace CecilStLabs
{

   enum eInternalEventType
   {
      IEVT_NONE                = 0,
      IEVT_GENERIC,

      IEVT_SETCONFIG
      // add additional event types to the bottom of the enumeration here
   };

   typedef struct
   {
      eInternalEventType type;                  // type of event
      int                size;                  // size of data in data buffer, will include NULL if string
      uint8_t            data[MAX_EVENT_DATA];  // place holder space for data - maybe not all used or sent
   } IEVENT_DATA;

   /**
    * API to handle internal events
    */
   class IInternalEvent
   {
      public:

         /*
          * getters for member data
          */
         inline eInternalEventType getEventType() { return m_data.type; }
         inline IEVENT_DATA* getData() { return &m_data; }
         inline int getDataSize() { return m_data.size; }
         inline uint8_t* getDataBuffer() { return m_data.data; }

         /**
          * copies the data for the event into its internal buffer
          *
          * @param data buffer of data
          * @param size size of the data, if its string data this includes NULL
          */
         virtual void setData( const uint8_t* data, const int size )
         {
            memset( m_data.data, 0, MAX_EVENT_DATA );
            memcpy( m_data.data, data, size );
            m_data.size = size;
         }

      protected:

         /**
          * NOTE: the constructor is protected to ensure that this abstract
          *       object cannot be created.
          */
         IInternalEvent() {};

         /**
          * NOTE: the destructor is protected to ensure that it cannot be called
          *       (since it cannot be created).
          */

         virtual ~IInternalEvent() {};


         /**
          * event type of this instance
          */
         eInternalEventType m_eventType;

         /**
          * data for this event
          */
         IEVENT_DATA m_data;


   };
}
#endif
