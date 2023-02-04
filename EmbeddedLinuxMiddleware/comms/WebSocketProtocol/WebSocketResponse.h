#ifndef WEBSOCKETRESPONSE_H_
#define WEBSOCKETRESPONSE_H_

namespace CecilStLabs
{

   /**
    * Enumeration containing the HTTP responses.
    * NOTE: STATUS_UNKNOWN is not a valid HTTP response status code but is provided
    *       if there was an error parsing the response, UNKNOWN is the default
    *       value for this application.
    */
   enum WS_HTTP_STATUS_CODES_e
   {
      WS_STATUS_UNKNOWN                = 0,
      WS_STATUS_OK                     = 200,
      WS_STATUS_ERROR                  = 500,
      WS_STATUS_TIMEOUT                = 501,
    };
   
   
   /**
    * Response from a Web Socket (not necessarily a request).
    * This is
    */
   class WebSocketResponse : public ICommProtocolResponse
   {
      public:

         /**
          * Create the default response to a websocket request.
          */
         WebSocketResponse();

         /**
          * Release any resources consumed by
          */
         virtual ~WebSocketResponse();

         /**
          * Parses the specified web socket response for
          * easy consumption.
          *
          * @param response The body of the response.
          *
          * @return True if the parse was successful, False if an error occurred
          *         attempting to parse the response.
          */
         bool parse(std::string response);

         /**
          * Clears out the last response and message.
          */
         void clear();

         /**
          * Sets the response object if the request timed out.
          */
         void requestTimedOut();
         
         /**
          * Returns the WS HTTP response code from the request.
          */
         WS_HTTP_STATUS_CODES_e getStatusCode() { return m_statusCode; }
         
      protected:
         
         WS_HTTP_STATUS_CODES_e m_statusCode;
         
      
  };

}
#endif
