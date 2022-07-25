#ifndef ICOMMPROTOCOLRESPONSE_H_
#define ICOMMPROTOCOLRESPONSE_H_

namespace CecilStLabs
{
   /**
    * Interface for parsing a response from a layer in the communication protocol request.
    */
   class ICommProtocolResponse
   {
      public:

         /**
          * Parse the response from the protocol.
          *
          * @param The full response body to parse.
          *
          * @return true if the response was parsed successfully,
          */
         virtual bool parse(std::string) = 0;

         /**
          * Returns the HTTP message body from the request.
          *
          * @return string containing the body of the message response.
          */
         inline std::string getMessageBody() { return m_messageBody; };

      protected:
         // NOTE: define the constructor, copy constructor, assignment
         //       operator, and destructor as protected to ensure this abstract
         //       interface cannot be instantiated.
         ICommProtocolResponse() {};
         virtual ~ICommProtocolResponse() {};
         ICommProtocolResponse(ICommProtocolResponse* copy __attribute__((unused))) {};
         ICommProtocolResponse* operator=(ICommProtocolResponse* copy __attribute((unused))) { return NULL; };

         /**
          * Contains the requested data.
          */
         string m_messageBody;
   };

}

#endif
