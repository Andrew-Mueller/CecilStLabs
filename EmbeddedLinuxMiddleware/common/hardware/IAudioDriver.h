#ifndef IAUDIODRIVER_H_
#define IAUDIODRIVER_H_

namespace CecilStLabs
{
   /**
    * Audio Driver interface to provide abstraction for audio devices in the system.
    */
   class IAudioDriver
   {
      public:
         /**
          * Turn audio on.
          */
         virtual void on() = 0;

         /**
          * Turn audio off.
          */
         virtual void off() = 0;

      protected:

         // NOTE: define the default constructor, copy constructor, destructor and
         //       assignment operator so the compiler doesn't provide defaults.
         IAudioDriver() { };
         IAudioDriver(IAudioDriver& copy __attribute__((unused))) {};
         virtual ~IAudioDriver() {};

         IAudioDriver& operator=(IAudioDriver& rhs __attribute__((unused))) { return rhs; }
   };
}

#endif
