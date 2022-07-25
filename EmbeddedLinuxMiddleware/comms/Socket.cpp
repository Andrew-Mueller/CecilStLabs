#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//lint -esym(18,*)
#include <iostream>
#include <sstream>
using namespace std;

#include <arpa/inet.h>
#include <errno.h>
#include <pthread.h>                            //lint !e537

#include "../common/basicTypes.h"
#include "../common/util/BasicUtil.h"
#include "../common/RingBuffer/RingBuffer.h"
#include "../common/MemoryPool/MemoryPool.h"
#include "RXData/RXData.h"

#include "SocketError.h"
#include "SocketBase.h"
#include "Socket.h"

namespace CecilStLabs
{
   Socket::Socket()
      : SocketBase()
   {
      // intentionally left blank.
   }

   Socket::~Socket()
   {
      // intentionally left blank.
   }

   SocketError Socket::Connect(string destination_ip)
   {
      return SocketBase::Connect(destination_ip);
   }

   int16_t Socket::SockWrite(const uint8_t* const buffer, const uint16_t numBytes)
   {
      int16_t bytesSent = 0;

      if (m_socket > 0)
      {
         bytesSent = send(m_socket, buffer, numBytes, 0);
      }

      return bytesSent;
   }

   int16_t Socket::SockRead(uint8_t* const buffer, const uint16_t numBytes)
   {
      size_t bytes_read = 0;

      if (m_socket > 0)
      {
         bytes_read = recv(m_socket, buffer, RXData::MAX_RX_BYTES, 0);
      }

      return bytes_read;
   }

}

