#include "../../common/basicTypes.h"
#include "../../common/util/EnumByName.h"
#include "../../common/DateTime/DateTime.h"
#include "../../common/util/BasicUtil.h"
#include "../../common/guid.h"

#include "../../common/GPIO/IGPIODriver.h"
#include "../../common/hardware/IAudioDriver.h"

#include "../../common/threads/PeriodicThread.h"

#include "IAudioDriver.h"
#include "AudioPattern.h"

namespace CecilStLabs
{

   AudioPattern::AudioPattern(IAudioDriver& audioDriver, uint8_t secondsOn, uint8_t secondsOff)
      : PeriodicThread(1, ePeriodInterval_Seconds),
        m_audioDriver(audioDriver),
        m_secondsOn(secondsOn),
        m_secondsOff(secondsOff),
        m_duration(0),
        m_isOn(false),
        m_currentSeconds(1),
        m_totalSeconds(0),
        m_isRunning(false)
   {
      // intentionally left blank.
   }

   AudioPattern::~AudioPattern()
   {
      // intentionally left blank.
   }

   void AudioPattern::Start(uint8_t duration)
   {
      m_isOn = false;
      m_isRunning = true;
      m_duration = duration;
      m_currentSeconds = 1;
      m_totalSeconds = 0;
      m_audioDriver.off();
      PeriodicThread::Start();
   }

   void AudioPattern::Stop()
   {
      m_isOn = false;
      m_isRunning = false;
      m_currentSeconds = 1;
      m_audioDriver.off();

      PeriodicThread::Stop();
   }

   void AudioPattern::PeriodicAsyncWorker()
   {
      m_totalSeconds++;
      if( m_duration > 0 && m_totalSeconds >= m_duration )
      {
         Stop();
      }
      else
      {
         --m_currentSeconds;

         if (0 == m_currentSeconds && m_isRunning)
         {
            if (m_isOn)
            {
               m_isOn = false;
               m_currentSeconds = m_secondsOff;
               m_audioDriver.off();
            }
            else
            {
               m_isOn = true;
               m_currentSeconds = m_secondsOn;
               m_audioDriver.on();
            }
         }
      }
   }
}
