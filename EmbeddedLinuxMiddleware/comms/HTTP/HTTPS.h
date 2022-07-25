#ifndef HTTPS_H_
#define HTTPS_H_

namespace CecilStLabs
{

   /**
    * Hyper Text Transfer Protocol over a secure socket.
    */
   class HTTPS : public ICommProtocol
   {
      public:
         /**
          * Create the secure http session with the specified host.
          *
          * @param hostname_str The fully qualified host name to interact with.
          */
         HTTPS(std::string hostname_str);
         virtual ~HTTPS();

         /**
          * Perform the appropriate http request using the specified path and host name.
          *
          * @param eMethod HTTP_Method_e enumeration for the type of request to perform.
          * @param path_str The path on the server to request.
          * @param body_str The body of the message request.
          *
          * @return the HTTPResponse as a result of the http request.
          */
         bool sendRequest(uint32_t eMethod,
                          std::string path_str,
                          std::string body_str);

         /**
          * Read the response to a previously sent request.
          * This is a blocking read.
          *
          * @param expectedNumPackets The number of packets expected in the response.
          * @param timeoutSeconds the number of seconds to wait, can be 0
           */
         void parseResponse(uint8_t expectedNumPackets, uint8_t timeoutSeconds);

         /**
          * Performs any cleanup necessary to complete the request.
          */
         void completeRequest();

         /**
          * Return the hostname used by this session.
          *
          * @return the hostname of the server we are communicating with.
          */
         std::string getHostname();

         /**
          * Sets the secure session to use for the connection.
          *
          * @param secureSession The SSL/HTTPS session used for all secure
          *                      communications.
          */
         void setSecureSession(SecureSession& secureSession);

         /**
          * Get the response for the most recent request.
          *
          * @return The last HTTPResponse received from the most recent request.
          */
         inline ICommProtocolResponse* getResponse() { return &m_response; };

         /**
          * return connection state
          *
          * @return true is connected
          */
         bool IsConnected() { return true; }
         
         bool IsActive() { return true; }

      protected:

         /**
          * Build the HTTP GET message according to the protocol.
          *
          * @param path_str The path to get the data from.
          *
          * @return String containing the GET request.
          */
         std::string BuildGET(std::string path_str);

         /**
          * Build the HTTP POST message according to the protocol.
          *
          * @param path_str The path to post the data to.
          * @param data_str The data to post.
          *
          * @return String containing the POST request.
          */
         std::string BuildPOST(std::string path_str, std::string data_str);

         /**
          * The server's host name this HTTP session will interact with.
          */
         std::string m_hostname_str;

         /**
          * The server's IP address of the host name
          */
         std::string m_ip_address_str;

         /**
          * Secure Socket used to send and receive messages.
          */
         SecureSocket m_sec_socket;

         /**
          * The response from the most recent request.
          */
         HTTPResponse m_response;
   };
}
#endif
