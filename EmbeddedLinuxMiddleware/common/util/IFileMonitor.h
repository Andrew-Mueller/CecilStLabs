#ifndef IFILEMONITOR_H_
#define IFILEMONITOR_H_

namespace CecilStLabs
{
   /**
    * Interface to monitor a specific file for changes.
    * The mechanism for monitoring the file and getting notification is platform
    * specific, so the common code only interact through this interface.
    */
   class IFileMonitor
   {
      public:

         /**
          * Sets up the platform specific mechanism used for monitoring the file.
          */
         virtual void initialize() = 0;

         /**
          * performs the action required by the platform specific mechanism to
          * check if the file being monitored has changed.
          */
         virtual void monitor() = 0;

         /**
          * Releases or closes any resources used for monitoring the file.
          */
         virtual void destroy() = 0;

      protected:

         // define the constructor, copy constructor, destructor and assignment
         // operator to ensure a concrete instance of this interface can be created.
         IFileMonitor() { };
         IFileMonitor(IFileMonitor& copy __attribute__((unused)) ) { };
         virtual ~IFileMonitor() { };

         IFileMonitor* operator=(const IFileMonitor* copy __attribute__((unused)) ) { return this; };
   };
}

#endif
