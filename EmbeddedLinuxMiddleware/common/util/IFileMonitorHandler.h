#ifndef IFILEMONITORHANDLER_H_
#define IFILEMONITORHANDLER_H_

namespace CecilStLabs
{
   /**
    * Interface for the action to take when the file being monitored has detected
    * a change.  This provides for a natural separation between the mechanism used for
    * monitoring the platform specific file system changes, and the action to take
    * based on those changes (which may be platform independent).
    */
   class IFileMonitorHandler
   {
      public:
         /**
          * The action to perform when the file being monitored is modified.
          *
          * @param bytes The bytes that were read from the file.
          * @param numBytes The number of bytes read from file.
          */
         virtual void handle(uint8_t* bytesRead, uint16_t numBytesRead) = 0;

      protected:

         /**
          * specify the constructor, copy constructor, destructor and assignment
          * operator to ensure a concrete version of this interface cannot be created
          * but instead must be "realized" through a subclass.
          */
         IFileMonitorHandler() { };
         IFileMonitorHandler(IFileMonitorHandler* copy __attribute__((unused))) { };
         virtual ~IFileMonitorHandler() { };
         IFileMonitorHandler* operator=(const IFileMonitorHandler* copy __attribute__((unused))) { return this; };

   };
}


#endif
