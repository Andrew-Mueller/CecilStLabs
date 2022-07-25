#ifndef LINUXCLOCKDRIVER_H
#define LINUXCLOCKDRIVER_H

#ifdef __APPLE__

/**
 * Darwin doesn't have the necessary RTC support.  To support development
 * in this environment, just define the rtc structure and the
 * RTC_SET_TIME constant.
 */
struct rtc_time
{
   int tm_sec;
   int tm_min;
   int tm_hour;
   int tm_mday;
   int tm_mon;
   int tm_year;
   int tm_wday;     /* unused */
   int tm_yday;     /* unused */
   int tm_isdst;    /* unused */
};

const int RTC_SET_TIME = 0;

#endif

namespace CecilStLabs
{
   /**
    * Hardware Clock driver specifically for Linux variants.
    */
   class LinuxClockDriver : public IClockDriver
   {
      public:
         LinuxClockDriver(std::string clockDevicePath);
         virtual ~LinuxClockDriver();

         /**
          * Sets the hardware clock to the specified Date and Time.
          *
          * @param currentTime The Date and Time to the set the system clock.
          * @return True if the time was set successfully, False if an error.
          */
         bool set(DateTime& currentTime);

         /**
          * Gets the current time.
          *
          * @return DateTime containing the current date and time.
          */
         DateTime getCurrentTime();

         /**
          * The real time clock is expecting the number of years as an integer since 1900.
          */
         static const uint16_t RTC_YEAR_OFFSET = 1900;

         /**
          * The real time clock has the month of January specified by 0.
          */
         static const uint8_t RTC_MONTH_OFFSET = 1;

      private:

         /**
          * The path to the device driver for the real time clock on the system.
          */
         std::string m_clockDevicePath;

         /**
          * Set the Systems hardware real time clock to the date and time specified.
          */
         bool setHwClock(DateTime& currentTime);

         /**
          * Set the Systems kernel clock to the date and time specified.
          */
         bool setSysClock(DateTime& currentTime);
   };

}
#endif
