#ifndef SOCKET_H
#define SOCKET_H

namespace CecilStLabs
{
   /**
    * Basic Socket implementation
    */
   class Socket : public SocketBase
   {
      public:

         /**
          * Create an empty Socket object.
          */
         Socket();

         /**
          * release the resources consumed by the Socket object.
          */
         virtual ~Socket();

         /**
          * Negotiate the socket connection.
          */
         virtual SocketError Connect(std::string destination_ip);

      protected:

         /**
          * The low level write to the socket implementation
          *
          * @param buffer Bytes to write to the socket.
          * @param numBytes The number of bytes in the specified byte buffer to
          *        write.
          *
          * @return The actual number of bytes written to the socket.  -1
          *         indicates an error occurred and the "errno" variable is set
          *         to one of the errors specified in the "read" documentation:
          *         man7.org/linux/man-pages/man2/send.2.html
          */
         virtual int16_t SockWrite(const uint8_t* const buffer, const uint16_t numBytes);

         /**
          * The low level read from the socket implementation.
          *
          * @param buffer Bytes read from the socket are placed into this buffer.
          * @param numBytes The number of bytes to be read from the socket.
          *
          * @return The actual number of bytes read from the socket.  Less bytes
          *         may be read than requested, this simply means that only that
          *         number of bytes were available at this moment, and another
          *         read is required if you need more bytes. Zero bytes
          *         read means the end of file.  -1 indicates an error occurred
          *         and the "errno" variable is set to one of the errors
          *         specified in the "read" documentation:
          *         man7.org/linux/man-pages/man2/read.2.html
          */
         virtual int16_t SockRead(uint8_t* const buffer, const uint16_t numBytes);
   };
}
#endif
