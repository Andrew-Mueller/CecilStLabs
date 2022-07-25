#ifndef WEBSOCKETPROTOCOL_H_
#define WEBSOCKETPROTOCOL_H_

namespace CecilStLabs
{

   /**
    * Bi-directional communications utilizing a web socket.
    */
   class WebSocketProtocol : public ICommProtocol
   {
      public:

         /**
          * Create the websocket protocol object with the specified host.
          *
          * @param hostname_str The fully qualified host name to interact with.
          *
          * @param application_path The path to the web socket application to
          *        connect to.
          *
          * @param deviceAccessKey device access key.
          */
         WebSocketProtocol(std::string hostname_str,
                           std::string applicationPath,
                           std::string deviceAccessKey);

         /**
          * Release resources consumed by the websocket protocol.
          */
         virtual ~WebSocketProtocol();

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
         bool sendRequest(uint32_t protocolMethod,
                          std::string location,
                          std::string message);

         /**
          * Parses the response from the previously requested resource based on
          * the specific comm protocol rules.
          *
          * @param The number of packets that make up the response.
          * @param timeoutSeconds the number of seconds to wait, can be 0
          */
         void parseResponse(uint8_t expectedNumPackets, uint8_t timeoutSeconds);

         /**
          * Performs any cleanup necessary to complete the request.
          */
         void completeRequest();

         /**
          * Return the hostname used by the session
          *
          * @return the hostname of the server we are communicating with.
          */
         std::string getHostname();

         /**
          * Sets the secure session to use for the connection.
          * NOTE: un-implemented for the WebSocket.
          *       (seems like this is in the wrong place for the interface)
          *
          * @param secureSession The SSL session used for all secure
          *                      communications.
          */
         void setSecureSession(SecureSession& secureSession)  {};

         /**
          * Get the response for the most recent request.
          *
          * @return The last response received from the most recent request.
          */
         inline ICommProtocolResponse* getResponse() { return &m_response; };

         /**
          * return connection state
          * 
          * @return true is connected
          */
         bool IsConnected();
	 
         /**
          * return if there is an active websocket
          * 
          * @return true is active
          */
         bool IsActive();
         
         
      protected:

         /**
          * websocket implementation used for bi-directional communication.
          */
         WebSocket m_webSocket;

	 /** 
	  * websocket application path at endpoint
	  */
	 std::string m_applicationPath;
	 
         /**
          * cached url of the server we are connecting to.
          */
         std::string m_hostnameUrl;

         /**
          * Location of the server.
          */
         std::string m_ip_address_str;

         /**
          * The device access key used to make the connection.
          */
         std::string m_deviceAccessKey;

         /**
          * cached response from the last message sent.
          */
         WebSocketResponse m_response;
   };
}
#endif
