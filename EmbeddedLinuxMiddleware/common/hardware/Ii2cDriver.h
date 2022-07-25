#ifndef II2CDRIVER_H_
#define II2CDRIVER_H_

namespace CecilStLabs
{

   /**
    * Interface abstraction for the i2c driver
    */
   class Ii2cDriver
   {
      public:

         /**
          * Writes the specified byte to the specific slave address.
          *
          * @param slave_address address on the I2C bus to write to.
          *
          * @param registerAddress Register of the device to write to.
          *
          * @param value Value to write to the device's register.
          */
         virtual void write(const uint8_t slave_address, const uint8_t registerAddress, const uint8_t value) = 0;

         /**
          * Reads a byte from the specific slave address.
          *
          * @param slave_address Address on the I2C bus to read from.
          *
          * @param registerAddress Register on the device to read from.
          *
          * @return Value read from the device's specified register.
          */
         virtual uint8_t read(const uint8_t slave_address, const uint8_t registerAddress) = 0;

         /**
          * Bare bones I2C write command for devices that don't have a structured
          * register interface, but instead might be expecting a specific byte pattern.
          *
          * @param slave_address Address of the device on the I2C Bus.
          *
          * @param value Byte value to write to the device.
          */
         virtual void rawWrite(const uint8_t slave_address, uint8_t* inputBuffer, const uint8_t bytesToWrite) = 0;

         /**
          * Bare bones I2C read command for devices that don't have a structured
          * register interface, but instead might be expecting a value to simply be read
          * once addressed.
          *
          * @param slave_address Address of the device on the I2C Bus.
          *
          * @param outputBuffer Buffer of bytes the I2C device will place its values into.
          *                     NOTE: be careful here for buffer overruns, etc.
          *
          * @param bytesToRead The Number of bytes to read into the output buffer.
          *
          * @return The number of bytes read.
          */
         virtual uint8_t rawRead(const uint8_t slave_address, uint8_t* outputBuffer, const uint8_t bytesToRead) = 0;

      protected:

         // define the default constructor, destructor, copy constructor and
         // assignment operator to ensure the compiler doesn't produce them for us
         // and the Interface cannot be instantiated on its own.
         Ii2cDriver() {};
         virtual ~Ii2cDriver() {};
         Ii2cDriver(Ii2cDriver& copy __attribute__((unused))) {};
         Ii2cDriver& operator=(Ii2cDriver& rhs __attribute__((unused))) { return rhs; };
   };
}
#endif
