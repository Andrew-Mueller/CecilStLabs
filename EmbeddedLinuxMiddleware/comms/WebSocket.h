#ifndef WEBSOCKET_H_
#define WEBSOCKET_H_

namespace CecilStLabs
{
   // forward definition of the semaphore type.  This allows not needing to
   // add unnecessary header includes.
   typedef int sem_t;

    /**
     * Prototype Websocket used to demo the capability of bi-directional
     * communications from an embedded platform.
     */
    class WebSocket : public Socket, public ILoggable
    {
        public:

            /**
             * Create the web socket client to the specified web socket server.
             *
             * @param url Universal Resource Location of the server web socket.
             *
             * @param application_path The path to the web socket application to
             *                         connect to.
             */
            WebSocket(std::string url,
                      std::string application_path);

            /**
             * release the web socket resources.
             */
            virtual ~WebSocket();

            /**
             * Connect the web socket to the specified server ip address.
             *
             * @param destination_ip IP address of the
             *
             * @param deviceAccessKey Access Key used to connect to server.
             *
             * @return SocketError object containing the error.
             */
            SocketError Connect(std::string destination_ip,
                                std::string deviceAccessKey);

            /**
             * Close the currently open web socket connection.
             */
            void Close();

            /**
             * Send the web socket ping frame to the server.
             */
            void Ping();

            /**
             * Checks the connected state of the socket.
             *
             * @return True if the socket is still connected, false if it is not.
             */
            bool IsConnected();

            /**
             * Return the data from the ringbuffer owned by the web socket
             *
             * @param timeoutSeconds the number of seconds to wait for data
             *        before timeing out.
             *
             * @return RXData the data read from the socket.  Null if no data was read.
             */
            RXData* Read(const uint8_t timeoutSeconds);

            /**
             * Called by the receive thread to do all the work
             *
             * @return true if ready to call again, false if connection lost
             */
            bool WebsocketWorker();

        protected:

            /**
             * The number of seconds to wait for the connection before timing out.
             */
            static const uint8_t DEFAULT_TIMEOUT_SECS      = 2;

            /**
             * Maximum number of bytes to receive from the websocket.
             */
            static const int16_t MAX_RX_BYTES              = MAX_INT16_VALUE;

            /**
             * Maximum number of attempts to read bytes from the socket.
             */
            static const int16_t MAX_READ_ATTEMPTS		   = 10;

            /**
             * The default key string used to make the websocket connection.
             */
            static const std::string DEFAULT_KEY;

            /**
             * url of the web socket endpoint to connect to.  cached for inclusion in the
             * websocket upgrade header.
             */
            std::string m_url;

            /**
             * Path to the web socket application
             */
            std::string m_app_path;

            /**
             * The memory used for received data that will be placed in the
             * RingBuffer object.
             */
            uint32_t m_rxFrameAddys[SocketBase::NUM_FRAMES_IN_POOL];

            /**
             * The RingBuffer contains chunks from the memory pool.  When the
             * data is received from the websocket's receive thread, data is allocated from
             * the memory pool, and the location of that memory is added to the
             * ring buffer for use by the main operating thread.
             */
            RingBuffer m_rxFrameBuffer;

            /**
             * Semaphore used to protect manipulating data received from
             * the socket.
             */
            pthread_mutex_t m_rxSemaphore;

            /**
             * The thread id for the receive thread.
             */
            pthread_t m_recvThread;

            /**
             * Sends the HTTP header requesting upgrade to a websocket connection.
             *
             * @param deviceAccessKey The device access key to connect to the websocket endpoint.
             *
             * @return True if the header was sent successfully, False is an
             *         error occurred.
             */
            bool SendHeader(std::string deviceAccessKey);

            /**
             * Checks the response from the server to the request for an
             * upgrade to a websocket connection.
             *
             * @return True if the response matches the request header.
             *         False if data didn't match or an error occured.
             */
            bool CheckHeaderResponse();

            /**
             * The low level write to the web socket
             *
             * @param buffer The buffer of bytes to write to the web socket.
             * @param numBytes The number of bytes in the buffer to write.
             *
             * @return The number of bytes written to the socket.
             */
            int16_t SockWrite(uint8_t* buffer, uint16_t numBytes);

            /**
             * Return the specified received data to the memory pool.
             *
             * @param data The RXData buffer to return to the memory pool.
             */
            inline void FreeRxData(RXData* data) { m_rxFramePool.free(data); };

    };   //lint !e1712

   /**
    * Non-member function used for starting up the thread.
    * The pThread creation method requires a non-class member to startup.
    * This non-member function is passed an instance of the Websocket object
    *
    * @param Websocket object to call back to object
    */
   void* WebsocketWorkerThread(void* arg);
}

#endif
