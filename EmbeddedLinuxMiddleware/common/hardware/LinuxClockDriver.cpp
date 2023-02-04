#include <cstdlib>
#include <ctime>
#include <sys/time.h>

#include <string>
#include <sstream>
#include <iostream>

// darwin doesn't seem to have the linux RTC operating system header
#ifndef __APPLE__
#include <linux/rtc.h>
#endif

#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <errno.h>

#include "../../common/basicTypes.h"
#include "../../common/util/EnumByName.h"
#include "../../common/DateTime/DateTime.h"
#include "../../common/util/BasicUtil.h"

#include "../../common/DateTime/IClockDriver.h"

#include "LinuxClockDriver.h"

namespace CecilStLabs
{
   LinuxClockDriver::LinuxClockDriver(std::string clockDevicePath)
      : m_clockDevicePath(clockDevicePath)
   {
      // intentionally left blank
   }

   LinuxClockDriver::~LinuxClockDriver()
   {
      // intentionally left blank
   }

   bool LinuxClockDriver::setHwClock(DateTime& currentTime)
   {
      bool success = false;
      struct rtc_time rtcTime;

      rtcTime.tm_sec = currentTime.getSecond();
      rtcTime.tm_min = currentTime.getMinute();
      rtcTime.tm_hour = currentTime.getHour();
      rtcTime.tm_mday = currentTime.getDay();
      rtcTime.tm_mon = currentTime.getMonth() - RTC_MONTH_OFFSET;
      rtcTime.tm_year = currentTime.getYear() - RTC_YEAR_OFFSET;

      // set the value of the real time clock
      int rtcHandle = open(m_clockDevicePath.c_str(), O_RDONLY);

      if (-1 != rtcHandle)
      {
         int status = ioctl(rtcHandle, RTC_SET_TIME, &rtcTime);

         if (-1 == status)
         {
            std::cout << "Error setting the Real Time Clock " << errno << std::endl;
         }
         else
         {
            success = true;
         }

         close(rtcHandle);
      }
      else
      {
         std::cout << "Error opening handle to the Real Time Clock " << errno << std::endl;
      }

      return success;
   }

   bool LinuxClockDriver::setSysClock(DateTime& currentTime)
   {
      bool success = false;

      struct tm systemTime;
      systemTime.tm_sec = currentTime.getSecond();
      systemTime.tm_min = currentTime.getMinute();
      systemTime.tm_hour = currentTime.getHour();
      systemTime.tm_mday = currentTime.getDay();
      systemTime.tm_mon = currentTime.getMonth() - RTC_MONTH_OFFSET;
      systemTime.tm_year = currentTime.getYear() - RTC_YEAR_OFFSET;

      time_t seconds = mktime(&systemTime);

      struct timeval newTime;
      newTime.tv_sec = seconds;
      newTime.tv_usec = 0;

      int status = settimeofday(&newTime, NULL);
      if (-1 == status)
      {
         std::cout << "Error setting the system clock " << errno << std::endl;

         switch(errno)
         {
            case EFAULT:
            {
               std::cout << "tv or tz outside address space " << std::endl;
               break;
            }
            case EINVAL:
            {
               std::cout << "timezone or something else is invalid" << std::endl;
               break;
            }
            case EPERM:
            {
               std::cout << "Insufficient privilege" << std::endl;
               break;
            }
            default:
            {
               BasicUtil::Assert(false, __FILE__, __LINE__);
            }
         }
      }
      else
      {
         success = true;
      }

      return success;
   }

   bool LinuxClockDriver::set(DateTime& currentTime)
   {
      bool success = false;

      if (setHwClock(currentTime))
      {
         if (setSysClock(currentTime))
         {
            success = true;
         }
         else
         {
            std::cout << "Error setting the system clock." << std::endl;
         }
      }
      else
      {
         std::cout << "Error setting the real time clock." << std::endl;
      }

      return success;
   }

   DateTime LinuxClockDriver::getCurrentTime()
   {
      DateTime currentTime;

      // get the current time in the format yyyy-mm-ddTHH:mm:ssZ
      time_t rawtime;
      struct tm* timeinfo;

      // retrieve the current time from the kernel.
      time (&rawtime);
      timeinfo = localtime (&rawtime);

      // convert the timeinfo structure into the DateTime object.
      currentTime.setTimeValue(timeinfo->tm_hour,
                               timeinfo->tm_min,
                               timeinfo->tm_sec,
                               timeinfo->tm_mon + RTC_MONTH_OFFSET,
                               timeinfo->tm_mday,
                               timeinfo->tm_year + RTC_YEAR_OFFSET);

      return currentTime;
   }
}
