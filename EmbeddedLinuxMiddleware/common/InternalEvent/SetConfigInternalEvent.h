#ifndef SETCONFIGINTERNALEVENT_H_
#define SETCONFIGINTERNALEVENT_H_

namespace CecilStLabs
{    
   /**
    * Set Config  event type
    */   
   class SetConfigInternalEvent : public IInternalEvent
   {
      public:
         SetConfigInternalEvent( string name, string value ); 
         SetConfigInternalEvent( IEVENT_DATA& evtData ); 
         
         virtual ~SetConfigInternalEvent();
                
         /**
          * copies the string for the event into its internal buffer and adds NULL
          * 
          * @param data buffer of data
          * @param size size of the data read from event pipe
          */
         void setData( const uint8_t* data, const int size );
         
         /**
          * getters for the name/value
          */
         inline string getName() { return m_name; }
         inline string getValue() { return m_value; }
         
      protected:
         
         /**
          * splits the data buffer int name/value pair
          * TODO: does this belong here:  maybe move to the Util object.
          */
         void splitData();
         
         string m_name;
         string m_value;
   };
}
#endif
