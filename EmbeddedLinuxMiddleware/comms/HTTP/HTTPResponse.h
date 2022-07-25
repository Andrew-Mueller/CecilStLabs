#ifndef HTTPRESPONSE_H
#define HTTPRESPONSE_H

namespace CecilStLabs
{
   /**
    * Enumeration containing the valid HTTP protocol versions
    */
   enum HTTP_PROTOCOL_VERSION_e
   {
      HTTP_UNKNOWN   =  0,
      HTTP_0_9       =  9,
      HTTP_1_0       = 10,
      HTTP_1_1       = 11
   };

   /**
    * Enumeration containing the HTTP responses.
    * NOTE: STATUS_UNKNOWN is not a valid HTTP response status code but is provided
    *       if there was an error parsing the response, UNKNOWN is the default
    *       value for this application.
    */
   enum HTTP_STATUS_CODES_e
   {
      STATUS_UNKNOWN                = 0,

      STATUS_OK                     = 200,

      STATUS_BAD_REQUEST            = 400,
      STATUS_UNAUTHORIZED           = 401,
      STATUS_FORBIDDEN              = 403,
      STATUS_NOT_FOUND              = 404,
      STATUS_REQUEST_TIMEOUT        = 408,

      STATUS_INTERNAL_SERVER_ERROR  = 500
   };

   /**
    * Response from an HTTP request.
    */
   class HTTPResponse : public ICommProtocolResponse
   {
      public:
         HTTPResponse();
         virtual ~HTTPResponse();

         /**
          * Parses the specified http response into its important parts for
          * easy consumption.
          *
          * @param response The raw http response string returned from a request.
          *
          * @return True if the parse was successful, False if an error occurred
          *         attempting to parse the response.
          */
         bool parse(std::string response);

         /**
          * Clears out the last response and message
          */
         void clear();

         /**
          * Sets the response object if the request timed out.
          */
         void requestTimedOut();

         /**
          * Returns the HTTP response code from the request.
          */
         HTTP_STATUS_CODES_e getStatusCode();

      protected:

         /**
          * Resulting version of the HTTP protocol contained in the response.
          */
         HTTP_PROTOCOL_VERSION_e m_protocolVersion;

         /**
          * Resulting status code contained in the response.
          */
         HTTP_STATUS_CODES_e m_statusCode;

         /**
          * This is the interpretation of the status code.
          */
         string m_reasonPhrase;

         // TODO: if needed add general headers, response headers, and entity headers.
         //       these are being left off for now for simplicity.

         /**
          * Parses the status line from the HTTP response consisting of the
          * HTTP protocol version, the status code, and the reason phrase.
          *
          * @param statusLine_str String containing the response's status line.
          */
         void parseStatusLine(string statusLine_str);

         /**
          * Parses out the specific protocol version given the sub-string from
          * the response containing the protocol version (example: "HTTP/1.1").
          * Populates the member variable m_protocolVersion.
          *
          * @param protocol_str Substring containing the protocol version from
          *                     the response.
          */
         void parseProtocolVersion(string protocol_str);

         /**
          * Parses out the status code given the sub-string from the response
          * containing the status code. (example: "404")
          * Populates the member variable m_statusCode.
          *
          * @param statusCode_str Substring containing the HTTP status code from
          *                       the response.
          */
         void parseStatusCode(string statusCode_str);

         /**
          * Find the double carriage-return line-feed in the string specified.
          *
          * @param str The string to find the back-to-back CRLF in.
          * @return The position where the two CRLF are found in the string.
          */
         size_t findDoubleLineBreak(string str);
   };
}

#endif
