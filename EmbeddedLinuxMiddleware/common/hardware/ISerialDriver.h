#ifndef ISERIALDRIVER_H_
#define ISERIALDRIVER_H_

namespace CecilStLabs
{
   /**
    * Interface to abstract a serial device driver from the actual physical
    * device.
    */
   class ISerialDriver
   {
      public:

         /**
          * Sends the specified byte buffer.
          * @param txBuffer The buffer containing the bytes to send.
          * @param numberOfBytes The number of bytes to send.
          * @return The actual number of bytes sent.
          */
         virtual uint16_t Send(uint8_t* txBuffer,
                               uint16_t numberOfBytes) = 0;

         /**
          * Receive the number of bytes specified and read them into the rxBuffer.
          * @param rxBuffer The buffer to read the bytes into.
          * @param numberOfBytes The number of bytes to expect to read.
          * @return The actual number of bytes read.
          */
         virtual uint16_t Receive(uint8_t* rxBuffer,
                                  uint16_t numberOfBytes) = 0;

      protected:

         // ensure the concrete version of the interface cannot be created.
         ISerialDriver() { }
         ISerialDriver(ISerialDriver& copy) { }
         virtual ~ISerialDriver() { }

         ISerialDriver& operator=(ISerialDriver& copy) { return copy; }
   };

}

#endif
