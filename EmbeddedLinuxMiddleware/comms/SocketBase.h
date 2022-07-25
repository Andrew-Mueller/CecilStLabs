#ifndef SOCKETBASE_H
#define SOCKETBASE_H

namespace CecilStLabs
{
   // forward definition of the semaphore type.  This allows not needing to
   // add unnecessary header includes.
   typedef int sem_t;

   /**
    * The Socket provides asynchronous reading and synchronous writing
    * from/to a destination.
    *
    * TODO: describe the design of the Socket here.  This will be extracted into
    *       the design documentation.
    */
   class SocketBase
   {
      public:

         /**
          * Connect the Socket to the server
          * @param destination_ip the IP address of the server to connect to.
          *
          * @return Error object enumeration describing the error that occurred or
          *         connection_success if Connection is made.
          */
         virtual SocketError Connect(std::string destination_ip);

         /**
          * Close the currently open Socket connection.
          */
         virtual void Close();

         /**
          * Sets the session's timeout, how long it should wait for data before
          * giving up.
          * The default is to wait forever.
          *
          * @param seconds the number of seconds before timing out a receive.
          * @return True if the timeout is successfully set, False if not.
          */
         bool SetRxTimeout(uint8_t seconds);

         /**
          * Sets the session's timeout in micro seconds instead of seconds.
          *
          * @param seconds The number of whole seconds to wait for the timeout.
          * @param micro_seconds The number of micro seconds to wait
          * @return True if the timeout is successfully set, False if not.
          */
         bool SetRxTimeout(uint8_t seconds, uint16_t micro_seconds);

         /**
          * Send the specified message to the http server.
          * @param message Data to send to the server.
          * @return True if the message was sent successfully, False if an error occurred.
          */
         virtual bool Send(std::string message);

         /**
          * Read the message from the http server.
          *
          * @param timeoutSeconds the number of seconds to wait for data
          *        before timeing out.
          *
          * @return RXData the data read from the socket.  Null if no data was read.
          */
         RXData* Read(const uint8_t timeoutSeconds);

         /**
          * Returns the specified rxData
          * @param RXData received data to be returned to the memory pool.
          */
         void FreeReadData(RXData* rxData);

         /**
          * Checks the connected state of the socket.
          *
          * @return True if the socket is still connected, false if it is not.
          */
         virtual bool IsConnected();

         /**
          * Checks the connected state of the actual ethernet connection.
          *
          * @return True if it is connected, false if it is not.
          */

         bool IsEthernetConnected();

         /**
          * The default port to use when creating a new connection.
          */
         static const uint16_t DEFAULT_PORT = 80;

      protected:
         /**
          * Size of the received frame memory pool.
          */
         static const uint8_t NUM_FRAMES_IN_POOL = 100;

         /**
          * The number of seconds to wait for data to read before giving up.
          */
         static const uint8_t RX_TIMEOUT_SECONDS = 10;

         /**
          * Number of bytes in the payload.
          */
         uint8_t m_payloadByteLength;

         /**
          * The IP address of the destination server.
          */
         std::string m_dst_ip;

         /**
          * Destination port to connect to.
          */
         uint16_t m_port;

         /**
          * Socket for network communication.
          */
         socket_t m_socket;

         /**
          * Concrete memory where the memory pool will operate with.
          */
         RXData m_rxFrames[NUM_FRAMES_IN_POOL];

         /**
          * The statically allocated pool of received frames used instead of
          * dynamically allocated memory.  This avoids memory fragmentation
          * as well as provides a deterministic upper bound.
          */
         MemoryPool<RXData> m_rxFramePool;

         /**
          * Semaphore used to protect socket operations across threads.
          */
         pthread_mutex_t m_socketSemaphore;

         /**
          * Semaphore used to keep debug print output strings from stomping on top of
          * each other (debug print cout and printf are not re-entrant and uses internal
          * variables to do its work.
          */
         pthread_mutex_t m_printSem;

         /**
          * internal flag to help signal the connected state of the socket.
          */
         bool m_isConnected;

         /**
          * currently *unused*, this flag is important when utilizing a separate
          * thread used for receiving data asynchronously (currently removed,
          * but will need to be re-added when web-sockets are fully implemented).
          */
         bool m_waitingForEndOfData;

         /**
          * The low level write to the socket implementation.
          *
          * @param buffer Buffer of bytes written to the socket
          * @param numBytes The number of bytes in the buffer.
          *
          * @return Number of bytes written to the socket.
          */
         virtual int16_t SockWrite(uint8_t* buffer, uint16_t numBytes) = 0;

         /**
          * The low level read from the socket implementation.
          *
          * @param buffer Buffer of bytes to put the read bytes into.
          * @param numBytes The number of bytes received from the socket and
          *                 placed into the buffer.
          *
          * @return Number of bytes written to the socket.
          */
         virtual int16_t SockRead(uint8_t* buffer, uint16_t numBytes) = 0;

         /**
          * This needs to be an abstract base class so do not allow creating an
          * instance of this abstract base class, destroying it with the
          * destructor, creating an instance using the default copy constructor,
          * or creating an instance using the assignment operator.
          */
         SocketBase();
         virtual ~SocketBase();

         /**
          * See note above.  Do not allow the creation of this abstract class
          * using the copy constructor.  This is defined here to make sure the
          * copy constructor cannot be created automatically by the compiler.
          * This method shouldn't be called except by accident.
          *
          * @param copy Instance of the SocketBase to copy member data from.
          */
         SocketBase(SocketBase* copy);

         /**
          * See the note above.  Do not allow the creation of this abstract class
          * using the assignment operator.  This is defined here to make sure the
          * default assignment operator is not created by the compiler.
          * This method shouldn't be called except by accident.
          *
          * @param copy The copy of the socket base to copy data to this
          *             instance from.
          *
          * @return Instance of a SocketBase containing the data of the 'copy'
          *         instance specified.
          */
         SocketBase* operator=(SocketBase* copy);

   };   //lint !e1712
}
#endif
