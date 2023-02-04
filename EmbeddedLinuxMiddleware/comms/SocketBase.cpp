#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

//lint -esym(18,*)
#include <iostream>
#include <sstream>
using namespace std;

#include <arpa/inet.h>
#include <errno.h>
#include <pthread.h>                            //lint !e537
#include <sys/ioctl.h>
#include <net/if.h>

#include "../common/basicTypes.h"
#include "../common/util/BasicUtil.h"
#include "../common/RingBuffer/RingBuffer.h"
#include "../common/MemoryPool/MemoryPool.h"

#include "RXData/RXData.h"
#include "SocketError.h"
#include "SocketBase.h"

#define TX_BUFF_SIZE    512
#define RX_BUFF_SIZE    1024

#undef DEBUG_PRINT

// TODO: Add logging functionality for error cases.
// TODO: detection and monitoring the connection and re-establish if needed (does this go here, or somewhere else)
// TODO: Deal with the underlying socket getting closed.  (is there a signal that can be caught).
// TODO: perform analysis for re-entrant and shared resources that need to be protected by critical sections.

namespace CecilStLabs
{
   SocketBase::SocketBase()
         : m_payloadByteLength(0),
           m_dst_ip("0.0.0.0"),
           m_port(DEFAULT_PORT),
           m_socket(0),
           m_rxFrames(),
           m_rxFramePool(m_rxFrames, NUM_FRAMES_IN_POOL),
           m_socketSemaphore(),
           m_printSem(),
           m_isConnected(false),
           m_waitingForEndOfData(false)

   {
      // initialize the socket semaphore
      BasicUtil::Assert((0 == pthread_mutex_init(&m_socketSemaphore, NULL)), __FILE__, __LINE__);

      // TODO: mutex to keep debug printing from stomping on each other.
      BasicUtil::Assert((0 == pthread_mutex_init(&m_printSem, NULL)), __FILE__, __LINE__);
   }

   SocketBase::SocketBase(SocketBase* copy)
      : m_payloadByteLength(0),
        m_dst_ip("0.0.0.0"),
        m_port(DEFAULT_PORT),
        m_socket(0),
        m_rxFrames(),
        m_rxFramePool(m_rxFrames, NUM_FRAMES_IN_POOL),
        m_socketSemaphore(),
        m_printSem(),
        m_isConnected(false),
        m_waitingForEndOfData(false)
   {
      // NOTE: this implementation is private and cannot be called.
      // It is here to satisfy the compiler and to make sure the compiler does
      // not create a default copy constructor by default.

      // should never be called, so halt if they are called by accident.
      BasicUtil::Assert(false, __FILE__, __LINE__);
   }

   SocketBase* SocketBase::operator=(SocketBase* copy)
   {
      // NOTE: this implementation is private and cannot be called.
      // It is here to satisfy the compiler and to make sure the compiler does
      // not create a default assignment operator by default.

      // should never be called, so halt if they are called by accident.
      BasicUtil::Assert(false, __FILE__, __LINE__);

      return (SocketBase*)NULL;
   }

   SocketBase::~SocketBase()
   {
      if (0 != m_socket)
      {
         Close();    //lint !e1551
      }

      // NOTE: sem_destroy returns a -1 if threads are still waiting at the semephore.
      //       ... this seems bad to spin here...

      int status = -1;

      while (-1 == status)
      {
         pthread_yield();     //lint !e1551
         usleep(1000);        //lint !e1551
         status = pthread_mutex_destroy(&m_socketSemaphore);
      }

      pthread_mutex_destroy(&m_printSem);
   }

   SocketError SocketBase::Connect(string destination_ip)
   {
      int status = 0;

      SocketError error(SocketError::connection_success);

      m_dst_ip = destination_ip;
      if (MUTEX_SUCCESS == pthread_mutex_lock(&m_socketSemaphore))
      {
         m_socket = socket(AF_INET, SOCK_STREAM, 0);
         pthread_mutex_unlock(&m_socketSemaphore);
      }

      if(m_socket <= 0)
      {
         // error trying to create the socket.
#ifdef DEBUG_PRINT
         cout << "Error creating the socket" << endl;
#endif
         error.setError(SocketError::error_creating_socket);
      }
      else
      {
         // setup the reading of the socket to timeout if nothing was received
         // for the specified number of seconds.
         if (!SetRxTimeout(RX_TIMEOUT_SECONDS))
         {
#ifdef DEBUG_PRINT
            cout << "Error setting the receive timeout" << endl;
#endif
            error.setError(SocketError::error_failed_setting_rx_timeout);
         }
         else
         {

            // The address to connect to for the socket.
            struct sockaddr_in serv_addr;
            (void)memset(&serv_addr, '0', sizeof(serv_addr));

            // specify the "internetwork" (sic) socket family (TCP in our case) and
            // using the port that we setup in the constructor.
            serv_addr.sin_family = AF_INET;
            serv_addr.sin_port = htons(m_port);

            // fill in the structure for the server's ip address structure for use
            // when creating the connection to the server.
            status = inet_pton(AF_INET, m_dst_ip.c_str(), &serv_addr.sin_addr);

            cout << "Server IP is: " << m_dst_ip << endl << endl;
            if (status <= 0)
            {
#ifdef DEBUG_PRINT
               cout << "inet_pton error occurred" << endl;
#endif
               error.setError(SocketError::error_init_ip_address);
            }
            else
            {
               // reset the status - if we don't get the lock, we want it to fail.
               status = -1;
               if (MUTEX_SUCCESS == pthread_mutex_lock(&m_socketSemaphore))
               {
                  // attempt to connect the socket to the server as specified in the
                  // serv_addr buffer (setup above).
                  status = connect(m_socket, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
                  pthread_mutex_unlock(&m_socketSemaphore);
               }

               if (status < 0)
               {
#ifdef DEBUG_PRINT
                  cout << "\n Error : Connect Failed" << status <<  endl;
#endif
                  error.setError(SocketError::error_connecting_socket);
               }
               else
               {
                  // got all the way through all this so set flag
                  m_isConnected = true;
               }
            }
         }
      }

      if (error.getError() != SocketError::connection_success)
      {
         // an error was set above so close socket
         // TODO - danger calling just close if is a websocket will try to send some websocket crap
         //  so just close the base socket thing
         SocketBase::Close();
      }

      return error;
   }

   void SocketBase::Close()
   {
#ifdef DEBUG_PRINT
         std::cout << "SocketBase::Close() request the socket close the connection.." << std::endl;
#endif

      // it is now safe to close the tcp socket.
      if (0 != m_socket)
      {
         int status = close(m_socket);
         if (0 != status)
         {
            std::cout << "an error occurred while closing the socket! " << status
                      << " errno " << errno << " -tastic." << std::endl;

#ifdef DEBUG_PRINT
           if (EIO == errno)
            {
               cout << "An I/O error occurred while reading from or writing to the file system." << endl;
            }
            else if (EBADF == errno)
            {
               cout << "Not a valid file descriptor" << endl;
            }
            else if (EINTR == errno)
            {
               cout << "The close() function was interrupted by a signal." << endl;
            }
            else
            {
               cout << "Unknown error when closing the socket. " << endl;
            }
#endif
         }
      }

      m_isConnected = false;

      m_socket = 0;
   }

   bool SocketBase::Send(std::string message)
   {
      bool sendSuccess = false;
      int16_t bytesSent = 0;

      // NOTE: as soon as the client side stops reading from the socket,
      //       the .net implementation throws an exception that is handled by
      //       the OnError event handler.  The exception shows up as an End of File.
      //       We don't need to write anything back, but we do need to constantly read.
      //
      //       As soon as execution is halted on this side or a breakpoint is hit,
      //       this exception is thrown.

      // send the data out the socket
      if (MUTEX_SUCCESS == pthread_mutex_lock(&m_socketSemaphore))
      {
         bytesSent = SockWrite((uint8_t*)message.c_str(), message.length());

         pthread_mutex_unlock(&m_socketSemaphore);

         #ifdef DEBUG_PRINT

            if (MUTEX_SUCCESS == pthread_mutex_lock(&m_printSem))
            {
               cout << "SocketBase Sent " << dec << (int)bytesSent << " bytes: " << message << endl;

               pthread_mutex_unlock(&m_printSem);
            }
         #endif

         // consider the data sent successful if the number of bytes actually sent
         // matches the number of bytes requested to be sent.
         if (bytesSent >= (int16_t)message.length())
         {
           sendSuccess = true;
         }
      }
      else
      {
         // error getting the mutex for sending the data.
         // TODO: log the error
      }

      return sendSuccess;
   }

   void SocketBase::FreeReadData(RXData* rxData)
   {
      m_rxFramePool.free(rxData);
   }

   bool SocketBase::IsConnected()
   {
      // the single byte received, but actually just thrown away.  We are
      // using the receive function as a test to see if the socket is still open.
      char buf;
      int bytesRead = -1;

      if (MUTEX_SUCCESS == pthread_mutex_lock(&m_socketSemaphore))
      {
         // TODO: does this work for secure socket?
         bytesRead = recv(m_socket, &buf, 1, MSG_PEEK);

         pthread_mutex_unlock(&m_socketSemaphore);

         if(bytesRead < 0)
         {
            // TODO: add documentation about why ewouldblock is important here
            //       for checking the connection state of the socket.
            if(EWOULDBLOCK != errno)
            {
               m_isConnected = false;
            }
            else
            {
               m_isConnected = true;
            }
         }
      }

      return m_isConnected;
   }

   bool SocketBase::IsEthernetConnected()
   {
      struct ifreq ifr;
      memset(&ifr, 0, sizeof(struct ifreq));

      // TODO: is this always the name?
      sprintf(ifr.ifr_name, "eth0");

      ioctl(m_socket, SIOCGIFFLAGS, (char *)&ifr);

      return (ifr.ifr_flags & IFF_RUNNING);

   }

   bool SocketBase::SetRxTimeout(uint8_t seconds)
   {
      return SetRxTimeout(seconds, 0);
   }

   bool SocketBase::SetRxTimeout(uint8_t seconds, uint16_t micro_seconds)
   {
      int status = 0;

      struct timeval timeout;
      timeout.tv_sec = seconds;
      timeout.tv_usec = micro_seconds;

      if (0 != m_socket)
      {
         // NOTE: success is indicated by 0, otherwise -1 on failure and "errno" is
         //       set with the error that occurred.
         //       It is up to the caller to take a look at errno.
         status = setsockopt(m_socket,
                             SOL_SOCKET,
                             SO_RCVTIMEO,
                             (const void*)&timeout,
                             sizeof(timeout));
      }


      #ifdef DEBUG_PRINT
         cout << "Timeout reset to " << (int)seconds << "." << (int)micro_seconds << endl;
      #endif

      return (0 == status);
   }

   RXData* SocketBase::Read(const uint8_t timeoutSeconds)
   {
      size_t bytes_read = 0;
      RXData* receivedData = NULL;

      // set the timeout to the number of seconds specified in the parameters
      // to this function.
      SocketBase::SetRxTimeout(timeoutSeconds);

      // NOTE: If the m_socketSemaphore is used to protect this block of code,
      //       a race condition exists between SockRead below blocking
      //       and other threads attempting to utilize the socket.
      //       this makes me unsure if the mutex is necessary.
      if (MUTEX_SUCCESS == pthread_mutex_lock(&m_socketSemaphore))
      {
         receivedData = m_rxFramePool.allocate();
         if (NULL != receivedData)
         {
            uint8_t* rxData = receivedData->GetDataPtr();
            memset (rxData, 0, RXData::MAX_RX_BYTES);

            // NOTE: The Socket Read here will block
            bytes_read = SockRead((uint8_t*)rxData, RXData::MAX_RX_BYTES);
            receivedData->SetNumBytes(bytes_read);

            pthread_mutex_unlock(&m_socketSemaphore);

            if (0 == bytes_read)
            {
               pthread_mutex_lock(&m_printSem);
               cout << "the socket was closed on the server end" << endl;
               pthread_mutex_unlock(&m_printSem);

               // Mark the connection as closed so we don't attempt to
               // use the connection or wait for any more data.
               m_isConnected = false;

               // danger
               //Close();
            }
            else if ((size_t)(-1) == bytes_read)
            {
               #ifdef DEBUG_PRINT
                  pthread_mutex_lock(&m_printSem);
                  cout << "no data is waiting to be read" << endl;
                  pthread_mutex_unlock(&m_printSem);
               #endif

               // we didn't read any data, but we allocated a frame.  So we need
               // to free the frame.
               m_rxFramePool.free(receivedData);
               receivedData = NULL;
            }
            else
            {
               #ifdef DEBUG_PRINT
                  pthread_mutex_lock(&m_printSem);
                  cout << "successful read! " << bytes_read << endl;
                  // this is an address
                  //cout << "byte read: " << (int)rxData << endl;

                  for (unsigned int i = 0; i < bytes_read; i++)
                  {
                     cout << rxData[i] << " ";
                  }

                  pthread_mutex_unlock(&m_printSem);
               #endif

               // NOTE: if we read less than the MAX_RX_BYTES, we can assume
               //       that we are done reading.

               // this test for a successful read is pretty simplistic
               // the data received will need to be tested at a higher level
               // to see if the data received was valid.
               if ((bytes_read <= 0) ||
                   (bytes_read > RXData::MAX_RX_BYTES))
               {
                  receivedData = NULL;
               }
               else
               {
                  // force a null termination at the end of the string.
                  if (bytes_read < RXData::MAX_RX_BYTES)
                  {
                     rxData[bytes_read] = 0;
                  }
                  else
                  {
                     rxData[RXData::MAX_RX_BYTES - 1] = 0;
                  }
               }
            }
         }
         else
         {
            cout << " failed to allocated RXData frame from mempool " << endl;
         }
      }

      // make sure to yield the scheduler to ensure the other threads have
      // a chance to run.
      pthread_yield();

      return receivedData;
   }

}

