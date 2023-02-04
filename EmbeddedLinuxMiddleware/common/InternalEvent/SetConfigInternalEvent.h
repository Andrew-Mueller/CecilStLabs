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
         SetConfigInternalEvent( std::string name, std::string value ); 
         SetConfigInternalEvent( IEVENT_DATA& evtData ); 
         
         virtual ~SetConfigInternalEvent();
                
         /**
          * copies the string for the event into its internal buffer and adds NULL
          * 
          * @param data buffer of data
          * @param size size of the data read from event pipe
          */
         void setData( const uint8_t* data, const uint8_t size );
         
         /**
          * getters for the name/value
          */
         inline std::string getName() { return m_name; }
         inline std::string getValue() { return m_value; }
         
      protected:
         
         /**
          * splits the data buffer int name/value pair
          * TODO: does this belong here:  maybe move to the Util object.
          */
         void splitData();
         
         std::string m_name;
         std::string m_value;
   };
}
#endif
