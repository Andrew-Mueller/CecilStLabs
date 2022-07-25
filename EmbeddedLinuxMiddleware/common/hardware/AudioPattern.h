#ifndef AUDIOPATTERN_H_
#define AUDIOPATTERN_H_

namespace CecilStLabs
{
   /**
    * A pattern of turning the audio driver on and off in a pattern, for example
    * one second on one second off.
    */
   class AudioPattern : public PeriodicThread
   {
      public:
         AudioPattern(IAudioDriver& audioDriver, uint8_t secondsOn, uint8_t secondsOff);
         virtual ~AudioPattern();


         // NOTE: implementation of the periodic async worker
         void Start( uint8_t duration = 0 );
         void Stop();
         void PeriodicAsyncWorker();

         bool isRunning() { return m_isRunning; }

      protected:

         /**
          * audio driver used to make some noise.
          */
         IAudioDriver& m_audioDriver;

         /**
          * The number of seconds the audio should be played for.
          */
         uint8_t m_secondsOn;

         /**
          * The number of seconds the audio should be silent for.
          */
         uint8_t m_secondsOff;

         /**
          * The number of seconds this pattern should run for (0 means forever).
          */
         uint8_t m_duration;

         /**
          * keeps track of the current state of the audio (on or off).
          */
         bool m_isOn;

         /**
          * The current number of seconds the audio state has been in.
          */
         uint8_t m_currentSeconds;

         /**
          * The total number of seconds the pattern has been running.
          */
         uint8_t m_totalSeconds;

         /**
          * Is the pattern running
          */
         bool m_isRunning;

   };

}
#endif
