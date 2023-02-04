
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

#include <cstdio>
#include <cstring>
#include <cerrno>
#include <unistd.h>
#include <arpa/inet.h>

using namespace std;

#include "IInternalEvent.h"
#include "SetConfigInternalEvent.h"

namespace CecilStLabs
{
   const char SET_DELIMITER = '|';
   const string SET_DELIMITER_STR = "|";
  
   SetConfigInternalEvent::SetConfigInternalEvent( string name, string value ) :
       m_name(name),
       m_value(value)
   {
      m_data.type = IEVT_SETCONFIG;
      
      string setting = m_name + SET_DELIMITER_STR + m_value;
      setData( (uint8_t*)setting.c_str(), setting.size() );
   }
   
   SetConfigInternalEvent::SetConfigInternalEvent( IEVENT_DATA& evtData )
   {
      m_name = "";
      m_value = "";
      m_data.type = IEVT_SETCONFIG;
      
      setData( evtData.data, evtData.size );
      splitData();
      
   }

   SetConfigInternalEvent::~SetConfigInternalEvent()
   {
      // intentionally left blank
   }
   
   void SetConfigInternalEvent::setData( const uint8_t* data, const uint8_t size )
   {
      if( size >= MAX_EVENT_DATA )
      {
         // truncate data, its too big
         memcpy( m_data.data, data, MAX_EVENT_DATA-1 );
         m_data.data[MAX_EVENT_DATA-1] = 0;
         m_data.size = MAX_EVENT_DATA;
      }
      else
      {
         memcpy( m_data.data, data, size );
         m_data.data[size] = 0;
         // size includes NULL
         m_data.size = size + 1;
      }
   }
   
   void SetConfigInternalEvent::splitData() 
   {
      // TODO: this needs to get refactored and put into the util class.
      
      // string str( (char*)m_data.data );
      string s = "";
      string& s1 = s;
      stringstream ss(s1);
      string item;
      vector<string> elems;
      
      while( getline( ss, item, SET_DELIMITER ) ) 
      {
         elems.push_back(item);
      }
      
      // should have found 2... name/value
      if( elems.size() == 2 )
      {
         m_name = elems[0];
         m_value = elems[1];
      }
   }
}
