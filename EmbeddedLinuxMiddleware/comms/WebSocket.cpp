#include <cstdio>
#include <cstdlib>
#include <cstring>

//lint -esym(18,*)
#include <iostream>
#include <sstream>

#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>
#include <errno.h>
#include <pthread.h>                            //lint !e537

#include "../common/basicTypes.h"
#include "../common/util/EnumByName.h"
#include "../common/DateTime/DateTime.h"
#include "../common/util/BasicUtil.h"

#include "../common/RingBuffer/RingBuffer.h"
#include "../common/MemoryPool/MemoryPool.h"

#include "../common/threads/PeriodicThread.h"

#include "RXData/RXData.h"
#include "SocketError.h"
#include "SocketBase.h"
#include "Socket.h"

#include "../common/logging/LogEntry.h"
#include "../common/logging/ILogDriver.h"
#include "../common/logging/emptyLogDriver.h"
#include "../common/logging/ILoggable.h"

#include "WebSocketProtocol/WebSocketFrame.h"
#include "WebSocket.h"

using namespace std;

// TODO: Add check for the header response (currently empty).
// TODO: Add support for fragmented messages (more than one packet).
// TODO: detection and monitoring the connection and re-establish if needed
// TODO: Deal with the underlying socket getting closed.
// TODO: perform analysis for re-entrant and shared resources that need to be protected by critical sections.

namespace CecilStLabs
{
   // As documented in the Websocket specification, this key is used utilized to xor
   const std::string WebSocket::DEFAULT_KEY("dGhlIHNhbXBsZSBub25jZQ==");

   WebSocket::WebSocket(std::string url,
                        std::string application_path)
         : Socket(),
           m_url(url),
           m_app_path(application_path),
           m_rxFrameAddys(),
           m_rxFrameBuffer(m_rxFrameAddys, (uint32_t)sizeof(uint32_t), SocketBase::NUM_FRAMES_IN_POOL),
           m_recvThread()
   {
      // initialize the received data semaphore
      BasicUtil::Assert((0 == pthread_mutex_init(&m_rxSemaphore, NULL)), __FILE__, __LINE__);
   }

   WebSocket::~WebSocket()
   {
      Socket::Close();    //lint !e1551

      // NOTE: sem_destroy returns a -1 if threads are still waiting at the semephore.
      //       Use the max destroy attempt so this doesn't wait forever (bad
      //       thing to do in a destructor).

      int status = -1;
      uint8_t max_destroy_attempt = 3;
      while ((-1 == status) &&
             (--max_destroy_attempt > 0))
      {
         pthread_yield();     //lint !e1551
         usleep(1000);        //lint !e1551
         status = pthread_mutex_destroy(&m_rxSemaphore);
      }
   }

   SocketError WebSocket::Connect(std::string destination_ip,
                                  std::string deviceAccessKey)
   {
      SocketError err = SocketError::connection_success;

      SocketBase::Connect(destination_ip);

      if (!SendHeader(deviceAccessKey))
      {
         SocketBase::Close();
         getLogDriver()->Log(std::string("WebSocket::Connect - Error sending the handshake"), LoggingError);
         err = SocketError::error_sending_header;
      }
      else
      {
         // take a look at the response from sending the header to verify
         // the web socket hand shaking was correct.
         if (!CheckHeaderResponse())
         {
            SocketBase::Close();
            getLogDriver()->Log(std::string("WebSocket::Connect - Error with the handshake response."), LoggingError);
            err = SocketError::error_parsing_header_response;
         }
         else
         {
            // knock down the timeout here we don't need to wait long
            Socket::SetRxTimeout(1);

            // Do a read - this tests to see if the connection is REALLY there
            if( !WebsocketWorker() )
            {
               err = SocketError::error_connecting_socket;
               getLogDriver()->Log(std::string("WebSocket::Connect - error - other end closed"), LoggingError);
            }
            else
            {
               // put timeout back
               Socket::SetRxTimeout(RX_TIMEOUT_SECONDS);

               // Start the receive thread here.
               int status = 0;
               status = pthread_create(&m_recvThread, NULL, &WebsocketWorkerThread, this);
               BasicUtil::Assert((0 == status), __FILE__, __LINE__);

               getLogDriver()->Log(std::string("WebSocket::Connect - success"), LoggingError);

            }
         }
      }

      return err;
   }


   void WebSocket::Ping()
   {
      uint8_t txBuff[WebSocketFrame::NUM_BYTES_IN_PING_FRAME];
      WebSocketFrame frame;

      getLogDriver()->Log(std::string("sending websocket ping"), LoggingInformation);

      if (true == frame.buildPingFrame(txBuff, WebSocketFrame::NUM_BYTES_IN_PING_FRAME))
      {
         const int16_t bytesSent = Socket::SockWrite(txBuff, WebSocketFrame::NUM_BYTES_IN_PING_FRAME);

         if (bytesSent != WebSocketFrame::NUM_BYTES_IN_PING_FRAME)
         {
            getLogDriver()->Log(std::string("Error sending Web Socket ping"), LoggingError);
         }
      }
   }

   int16_t WebSocket::SockWrite(uint8_t* buffer, uint16_t numBytes)
   {
      // send the data out the socket
      int16_t bytesSent = -1;
      WebSocketFrame frame;

	  // TODO:
      // make sure there is at least one null terminator (most likely the
      // message has already been null terminated, but lets not take any chances.
      /*if ((NULL != buffer) && (numBytes > 0))
      {
         buffer[numBytes - 1] = '\0';
      }*/

      frame.setMessage((const char*)buffer);
      frame.encode();
      unsigned char encodedBuffer[MAX_UINT16_VALUE];
      memcpy(encodedBuffer, frame.getCodedMessage(), frame.getLengthOfCodedMessage());
      bytesSent = Socket::SockWrite(encodedBuffer, frame.getLengthOfCodedMessage());

      return bytesSent;
   }

   RXData* WebSocket::Read(const uint8_t timeoutSeconds)
   {
      rbNumElements_t numRead = 0;
      RXData* rxData = NULL;
      uint8_t curr_second = 0;

      // TODO: utilize poll to simulate a timeout on read
      //       stackoverflow.com/questions/9163308

      // TODO: implement the timeout in seconds.  currently it just blocks
      //       indefinitely.
      // block until data is received or the specified timeout is reached.
      while((timeoutSeconds >= curr_second++) &&
            (NULL == rxData))
      {
         for( int i = 0; i < MAX_READ_ATTEMPTS; i++ )
         {
            // loop through 10 times at about .1 sec per time.
            //  this makes it more responsive than 1 second granularity and can get out in as early as .1 sec
            usleep(100000);

            if (MUTEX_SUCCESS == pthread_mutex_lock(&m_rxSemaphore))
            {
               if (!m_rxFrameBuffer.isEmpty())
               {
                  uint32_t rxDataAddy = 0;
                  numRead = (uint32_t)m_rxFrameBuffer.get(&rxDataAddy, 1);

                  if (0 != numRead)
                  {
                     rxData = (RXData*)rxDataAddy;
                     pthread_mutex_unlock(&m_rxSemaphore);
                     break;
                  }
               }

               pthread_mutex_unlock(&m_rxSemaphore);
            }
         }
      }

      return rxData;
   }

   bool WebSocket::SendHeader(std::string deviceAccessKey)
   {
      // NOTE: the spec suggests \r\n to separate the lines of the request.
      //       The speedway responds to \r\n in a sprintf with a core dump.
      //       I can't find any documentation of why this might occur.
      ostringstream headerStr;
      headerStr << "GET " << m_app_path << "?AccessKey=" << deviceAccessKey << " HTTP/1.1" << endl;
      headerStr << "Host: " << m_url << endl;
      headerStr << "Upgrade: websocket" << endl;
      headerStr << "Connection: Upgrade" << endl;
      headerStr << "Sec-Websocket-Key: " << DEFAULT_KEY << endl;
      headerStr << "Sec-WebSocket-Version: 13" << endl << endl;


      stringstream requestStr;
      requestStr << endl << "WebSocket::SendHeader - " << (int)headerStr.str().length() << " bytes" << endl;
      requestStr << headerStr.str() << endl;
      getLogDriver()->Log(requestStr.str(), LoggingDebug);

      uint32_t status = 0;

      // NOTE: i feel like i should mutex protect the use of the m_socket object
      //       however, there is a race condition that can occur if the receive thread
      //       gets the mutex, and then blocks on the recv function (inside the critical section).
      //
      {
         status = Socket::SockWrite((uint8_t*)headerStr.str().c_str(), (int)headerStr.str().length());
         if (0 == status)
         {
            Socket::Close();
         }

         stringstream statusStr;
         statusStr << "WebSocket::SendHeader status: " << status << " bytes sent." << endl;
         getLogDriver()->Log(statusStr.str(), LoggingInformation);

         // see note above regarding lack of mutex...
      }

      if (status != (uint32_t)headerStr.str().length())
      {
         stringstream statusStr;
         statusStr << "WebSocket::SendHeader - sent less bytes than expected. sent: " << status <<
                 " expected: " << (int)headerStr.str().length() << endl;
         getLogDriver()->Log(statusStr.str(), LoggingInformation);
      }

      // TODO: is sending less bytes than expected an error?  Or do i manually need to handle sending the
      //       rest of the message?

      return (status == (uint32_t)headerStr.str().length());
   }

   bool WebSocket::CheckHeaderResponse()
   {
      bool success = false;
      uint8_t rxData[MAX_RX_BYTES];
      memset (rxData, 0, MAX_RX_BYTES);

      int16_t bytes_read = Socket::SockRead(rxData, MAX_RX_BYTES);

      if (bytes_read > 0)
      {
         stringstream responseStr;
         responseStr << "WebSocket::CheckHeaderResponse - " << rxData << endl;

         for (int i = 0; i < bytes_read; i++)
         {
            responseStr << rxData[i];
         }

         getLogDriver()->Log(responseStr.str(), LoggingInformation);

         // TODO: compare the response fields returned from the server against
         //       the values expected.

         success = true;
      }
      else
      {
         getLogDriver()->Log(std::string("WebSocket::CheckHeaderResponse - header no response"), LoggingError);
      }

      return success;
   }

   bool WebSocket::IsConnected()
   {
      return m_isConnected;
   }

   bool WebSocket::WebsocketWorker()
   {
      bool readSuccess = false;
      bool continueReadThread = true;
      int16_t totalBytesRead = 0;
      int16_t bytes_read = 0;
      RXData* receivedData = NULL;
      stringstream str;

      uint8_t rxData[MAX_RX_BYTES];
      memset (rxData, 0, MAX_RX_BYTES);

      // Semephore protect the rx buffer
      if (MUTEX_SUCCESS == pthread_mutex_lock(&m_rxSemaphore))
      {
         // pull a new frame from the memory pool.
         // NOTE: whoever consumes the data from the ring buffer will need to
         //       free the memory back to the memory pool.

         receivedData = Socket::m_rxFramePool.allocate();

         if (NULL != receivedData)
         {
            // the frame is available from the memory pool, go ahead and read
            // the data from the socket.
            pthread_mutex_unlock(&m_rxSemaphore);

            // Semaphore to protect the socket operation seems logical here, however
            // the recv is blocking, control would lock the mutex and then block on the
            // recv in this thread before being able to do anything else, making sends
            // impossible.

            {
               bytes_read = -1;
               errno = 0;

               str.clear();
               str << " WebSocket::WebsocketWorker - before Socket::SockRead" << endl;
               getLogDriver()->Log(str.str(), LoggingDebug);

               bytes_read = Socket::SockRead(rxData, MAX_RX_BYTES);

               str.clear();
               str << " WebSocket::WebsocketWorker - after Socket::SockRead - " << bytes_read << " bytes read" << endl;
               getLogDriver()->Log(str.str(), LoggingDebug);

               //int savedError = errno;
               totalBytesRead = bytes_read;

               if (0 == totalBytesRead)
               {
                  getLogDriver()->Log(std::string("WebSocket:SockRead - closed on other end"), LoggingError);
                  Socket::Close();

                  m_rxFramePool.free(receivedData);

                  continueReadThread = false;
               }
               else if (-1 == totalBytesRead)
               {

                  // this error means the other side was closed
                  if( ECONNRESET == errno )
                  {
                     getLogDriver()->Log(std::string("WebSocket:SockRead - closed on other end"), LoggingError);
                     Socket::Close();

                     continueReadThread = false;
                  }

                  m_rxFramePool.free(receivedData);
               }
               else
               {
                  int savedError;

                  // NOTE: shorten the receive timeout here as much as possible.
                  //       the next call to WebSocket::Send or WebSocket::Read
                  //       resets the receive timeout.
                  Socket::SetRxTimeout(0, 1);

                  // need to read again here in the case we didn't read all the bytes
                  // in the first read.  When we read the end of file (EOF) the number
                  // of bytes read will be zero and/or there will be an error raised
                  // via errno.
                  while ((bytes_read > 0) && (0 == savedError))
                  {
                     errno = 0;

                     str.clear();
                     str << " WebSocket::WebsocketWorker - before Socket::SockRead extended" << endl;
                     getLogDriver()->Log(str.str(), LoggingDebug);

                     bytes_read = Socket::SockRead(&rxData[totalBytesRead],
                                                   MAX_RX_BYTES);
                     str.clear();
                     str << " WebSocket::WebsocketWorker - after Socket::SockRead extended - " << bytes_read << " bytes read" << endl;
                     getLogDriver()->Log(str.str(), LoggingDebug);

                     savedError = errno;

                     if (0 == errno)
                     {
                        totalBytesRead += bytes_read;
                     }
                     else if( ECONNRESET == errno )
                     {
                        getLogDriver()->Log(std::string("WebSocket:SockRead - closed on other end"), LoggingError);
                        Socket::Close();

                        m_rxFramePool.free(receivedData);
                        continueReadThread = false;
                     }
                     else
                     {
                        getLogDriver()->Log(std::string("WebSocket:SockRead - Done Reading Extended WebSocket Data."), LoggingDebug);
                     }
                  }

                  // put timeout back
                  Socket::SetRxTimeout(RX_TIMEOUT_SECONDS);

                  // TODO: this test for a successful read seems pretty simplistic right now...
                  //       and possibly incorrect (it is valid for a socket to receive less data than is requested)
                  //       the test will need to be made at a higher level to see if the data
                  //       received was valid.
                  readSuccess = ((totalBytesRead > 0) && (totalBytesRead < MAX_RX_BYTES));
               }
            }
         }
         else
         {
            // the memory pool was empty.  Simply leave the data in the socket's
            // buffer until the next read attempt.  This continues until a frame
            // is returned to the memory pool (which should happen as soon as the
            // read data is processed.
            pthread_mutex_unlock(&m_rxSemaphore);
            getLogDriver()->Log(std::string("WebSocket:SockRead - Memory Pool of receive frame's is empty. Waiting to read until frames are returned to the memory pool."),
                                            LoggingError);
         }
      }

      // check if bytes were read, or if the read call timed out.
      if (readSuccess && continueReadThread)
      {
         receivedData->Clear();

         // decode received websocket message.
         WebSocketFrame frame;
         frame.setMessageBuffer(rxData, totalBytesRead);
         if (frame.decode())
         {
            receivedData->SetData((uint8_t*)frame.getMessage().c_str(), frame.getMessage().length());
         }
         else
         {
            getLogDriver()->Log(std::string("WebSocket:SockRead - Error decoding websocket frame"), LoggingError);
         }

         if (MUTEX_SUCCESS == pthread_mutex_lock(&m_rxSemaphore))
         {
            // stuff the address of the received frame into the Rx ring
            // buffer for consumption by the main thread.
            uint32_t rxDataAddy = (uint32_t)receivedData;
            m_rxFrameBuffer.put(&rxDataAddy, 1);
         }

         pthread_mutex_unlock(&m_rxSemaphore);
      }

      // make sure to yield the scheduler to ensure the other threads have
      // a chance to run.
      pthread_yield();

      return continueReadThread;
   }

   void* WebsocketWorkerThread(void* arg)
   {
      if (NULL != arg)
      {
         WebSocket* socketArg = (WebSocket*)arg;

         // perform the thread's requested work.
         // until false is returned which means connection lost
         while( socketArg->WebsocketWorker() );
      }

      return NULL;
   }
}

