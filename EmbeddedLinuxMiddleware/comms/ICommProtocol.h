#ifndef ICOMMPROTOCOL_H_
#define ICOMMPROTOCOL_H_

namespace CecilStLabs
{
   /**
    * Interface for a communication protocol for sending requests and receiving
    * responses based on those requests.
    */
   class ICommProtocol : public ILoggable
   {
      public:

         virtual ~ICommProtocol() {};

         /**
          * Request the specific resource using the protocol.
          *
          * @param protocolMethod The method of the request, expected to be a member of a
          *                       strongly typed method enumeration.
          * @param location The location to send the protocol request to.
          * @param message The payload of the request.
          *
          * @return True if the request succeeds, False if it fails.
          */
         virtual bool sendRequest(uint32_t protocolMethod,
                                  std::string location,
                                  std::string message) = 0;

         /**
          * Parses the response from the previously requested resource based on
          * the specific comm protocol rules.
          *
          * @param The number of packets that make up the response.
          * @param timeoutSeconds time to wait for data, can be 0
          */
         virtual void parseResponse(uint8_t expectedNumPackets, uint8_t timeoutSeconds = RESPONSE_TIMEOUT_SECONDS) = 0;

         /**
          * Performs any cleanup necessary to complete the request.
          */
         virtual void completeRequest() = 0;

         /**
          * Return the hostname used by the session
          *
          * @return the hostname of the server we are communicating with.
          */
         virtual std::string getHostname() = 0;

         /**
          * Sets the secure session to use for the connection.
          *
          * @param secureSession The SSL/HTTPS session used for all secure
          *                      communications.
          */
         virtual void setSecureSession(SecureSession& secureSession) = 0;

         virtual ICommProtocolResponse* getResponse() = 0;

         virtual bool IsConnected() = 0;
         virtual bool IsActive() = 0;

      protected:

         static const uint8_t RESPONSE_TIMEOUT_SECONDS = 10;

         // NOTE: define the constructor, copy constructor, assignment
         //       operator, and destructor as protected to ensure this abstract
         //       interface cannot be instantiated.
         ICommProtocol() {};
         ICommProtocol(ICommProtocol* copy __attribute__((unused))) {};
         ICommProtocol* operator=(ICommProtocol* copy __attribute((unused))) { return NULL; };
   };

}
#endif
