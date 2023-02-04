#include <iostream>
#include <sstream>
#include <string>
using namespace std;

#include "../../common/basicTypes.h"
#include "../../common/util/EnumByName.h"
#include "../../common/DateTime/DateTime.h"
#include "../../common/util/BasicUtil.h"

#include "../ICommProtocolResponse.h"
#include "WebSocketResponse.h"

namespace CecilStLabs
{
   WebSocketResponse::WebSocketResponse()
      : m_statusCode(WS_STATUS_UNKNOWN)
   {
      m_messageBody.clear();
   }

   WebSocketResponse::~WebSocketResponse()
   {
      // intentionally left blank.
   }

   bool WebSocketResponse::parse(std::string response)
   {
      // cache the response as the message body.
      m_messageBody = response;
      
      // parse out the status code
      if( !response.empty() )
      {
         if( string::npos != response.find( "200" ) )
         {
            m_statusCode = WS_STATUS_OK;
         }
         else if( string::npos != response.find( "500" ) )
         {
            m_statusCode = WS_STATUS_ERROR;
         }
      }

      return true;
   }

   void WebSocketResponse::clear()
   {     
      m_messageBody.clear();
      m_statusCode = WS_STATUS_UNKNOWN;
   }
   
   void WebSocketResponse::requestTimedOut()
   {
      m_statusCode = WS_STATUS_TIMEOUT;
      m_messageBody.clear();
   }

}
