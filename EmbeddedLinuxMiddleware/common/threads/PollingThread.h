#ifndef POLLINGTHREAD_H_
#define POLLINGTHREAD_H_

namespace CecilStLabs
{
   /**
    * Thread that polls some external object (file, pipe, input, etc.) to signal
    * doing it's work (instead of being periodic, etc.)
    *
    * TODO: currently unused and unimplemented...
    */
   class PollingThread
   {
      public:
         PollingThread();
         virtual ~PollingThread();
   };

}
#endif
