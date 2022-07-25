#include <stdlib.h>
#include <string>
using namespace std;

#include "../../common/basicTypes.h"
#include "../../common/util/BasicUtil.h"

#include "../ICommProtocolResponse.h"
#include "HTTPResponse.h"

namespace CecilStLabs
{
   HTTPResponse::HTTPResponse()
      : m_protocolVersion(HTTP_UNKNOWN),
        m_statusCode(STATUS_UNKNOWN)
   {
      // Intentionally left blank.
   }

   HTTPResponse::~HTTPResponse()
   {
      // intentionally left blank.
   }

   bool HTTPResponse::parse(string response)
   {
      bool parse_accomplished = false;

      // NOTE: helpful information about the message format of the HTTP Response:
      // www.tcpipguide.com/free/t_HTTPResponseMessageFormat.htm

      // start by resetting the necessary values.
      clear();

      if (!response.empty())
      {
         // NOTE: It would make sense to search for \r\n but the arm compiler
         //       causes core dumps for any string containing \r\n back to back.
         size_t breakPosition = response.find("\n");

         if (breakPosition != string::npos)
         {
            // the substring from (0 - breakPosition) should contain the status line.
            string statusLine_str = response.substr(0, breakPosition);
            parseStatusLine(statusLine_str);

            // Grab everything from \r\n\r\n to the end of the message
            // (add 4 characters to the beginning of the substring to make sure
            //  we don't get the leading two blank lines)

            // NOTE: can't use a simple find for \r\n\r\n here because of the
            //       bug in the arm compiler.  We have to be clever...
            breakPosition = findDoubleLineBreak(response);
            m_messageBody = response.substr(breakPosition + 4, response.length() - 1);

            // Ensure the status line was parsed successfully,
            // double check a valid status code and HTTP version were found
            if ((m_statusCode > 0) &&
                (m_protocolVersion > 0))
            {
               parse_accomplished = true;
            }
         }
      }

      return parse_accomplished;
   }

   void HTTPResponse::clear()
   {
      m_statusCode = STATUS_UNKNOWN;
      m_messageBody.clear();
   }

   void HTTPResponse::requestTimedOut()
   {
      m_statusCode = STATUS_REQUEST_TIMEOUT;
      m_messageBody.clear();
   }

   HTTP_STATUS_CODES_e HTTPResponse::getStatusCode()
   {
      return m_statusCode;
   }

   void HTTPResponse::parseStatusLine(string statusLine_str)
   {
      if (!statusLine_str.empty())
      {
         // The status line's format looks like:
         //
         // HTTP/1.1 200 OK
         // [protocol] space [status code] space [reason phrase]
         //
         // from the example above,
         // protocol = HTTP/1.1
         // status code = 200
         // reason phrase = OK

         // 1. find the first space separating the protocol and the status code.

         size_t protocolVerPos = statusLine_str.find(" ");

         if (string::npos != protocolVerPos)
         {
            parseProtocolVersion(statusLine_str.substr(0, protocolVerPos));

            // 2. find the second space separating the status code and the
            //    reason phrase
            size_t codePos = statusLine_str.find(" ", protocolVerPos + 1);

            if (string::npos != codePos)
            {
               parseStatusCode(statusLine_str.substr(protocolVerPos, codePos - protocolVerPos));

               // capture the reason phrase... for profit.
               m_reasonPhrase = statusLine_str.substr(codePos - protocolVerPos + 1, statusLine_str.length());
            }
         }
      }
   }

   void HTTPResponse::parseProtocolVersion(string protocol_str)
   {
      if (string::npos != protocol_str.find("HTTP/0.9"))
      {
         m_protocolVersion = HTTP_0_9;
      }
      else if (string::npos != protocol_str.find("HTTP/1.0"))
      {
         m_protocolVersion = HTTP_1_0;
      }
      else if (string::npos != protocol_str.find("HTTP/1.1"))
      {
         m_protocolVersion = HTTP_1_1;
      }
      else
      {
         m_protocolVersion = HTTP_UNKNOWN;
      }
   }

   void HTTPResponse::parseStatusCode(string statusCode_str)
   {
      // brute force the string, looking for the specific status codes
      // defined by the HTTP Spec, that this application understands.

      if (string::npos != statusCode_str.find("200"))
      {
         m_statusCode = STATUS_OK;
      }
      else if (string::npos != statusCode_str.find("400"))
      {
         m_statusCode = STATUS_BAD_REQUEST;
      }
      else if (string::npos != statusCode_str.find("401"))
      {
         m_statusCode = STATUS_UNAUTHORIZED;
      }
      else if (string::npos != statusCode_str.find("403"))
      {
         m_statusCode = STATUS_FORBIDDEN;
      }
      else if (string::npos != statusCode_str.find("404"))
      {
         m_statusCode = STATUS_NOT_FOUND;
      }
      else if (string::npos != statusCode_str.find("408"))
      {
         m_statusCode = STATUS_REQUEST_TIMEOUT;
      }
      else if (string::npos != statusCode_str.find("500"))
      {
         m_statusCode = STATUS_INTERNAL_SERVER_ERROR;
      }
      else
      {
         // NOTE: unknown is not a valid HTTP status code but is provided to
         //       signal that there was an error parsing the response.
         m_statusCode = STATUS_UNKNOWN;
      }
   }

   size_t HTTPResponse::findDoubleLineBreak(string str)
   {
      // NOTE: the monte-vista linux arm version of gcc produces instructions
      //       that cause core dumps if \r\n is used in a string.

      // We have to work around this limitation by making sure to always
      // separate the \r and the \n.

      size_t startPos = 0;

      size_t firstPos = 0;
      size_t secondPos = 0;

      while (2 != (secondPos - firstPos))
      {
         firstPos = str.find("\r", startPos);
         secondPos = str.find("\r", firstPos + 1);

         startPos = firstPos + 1;
      }

      return firstPos;
   }
}
