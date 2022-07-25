#include <cstdlib>
#include <ctime>

#include <string>
#include <sstream>
using namespace std;

#include "../basicTypes.h"
#include "../util/EnumByName.h"
#include "DateTime.h"

namespace CecilStLabs
{
   DateTime::DateTime()
      : m_hour(0),
        m_minute(0),
        m_second(0),
        m_month(1),
        m_day(1),
        m_year(0)
   {
      // intentionally left blank
   }

   DateTime::~DateTime()
   {
      // intentionally left blank
   }

   bool DateTime::operator==(const DateTime& rhs) const
   {
      if ((m_hour == rhs.m_hour) &&
          (m_minute == rhs.m_minute) &&
          (m_second == rhs.m_second) &&
          (m_month == rhs.m_month) &&
          (m_day == rhs.m_day) &&
          (m_year == rhs.m_year))
      {
         return true;
      }

      return false;
   }

   bool DateTime::operator!=(const DateTime& rhs) const
   {
      if (*this == rhs)
      {
         return false;
      }

      return true;
   }

   DateTime& DateTime::operator=(const DateTime& rhs)
   {
      if (*this != rhs)
      {
         m_hour = rhs.m_hour;
         m_minute = rhs.m_minute;
         m_second = rhs.m_second;
         m_month = rhs.m_month;
         m_day = rhs.m_day;
         m_year = rhs.m_year;
      }

      return *this;
   }

   void DateTime::setTimeValue(uint8_t hour,
                               uint8_t minute,
                               uint8_t second,
                               uint8_t month,
                               uint8_t day,
                               uint16_t year)
   {
      if (hour <= MAX_HOUR)
      {
         m_hour = hour;
      }

      if (minute <= MAX_MINUTE)
      {
         m_minute = minute;
      }

      if (second <= MAX_SECOND)
      {
         m_second = second;
      }

      if ((month >= MIN_MONTH) &&
          (month <= MAX_MONTH))
      {
         m_month = month;
      }

      if ((day >= MIN_DAY) &&
          (day <= MAX_DAY))
      {
         m_day = day;
      }

      if (year <= MAX_YEAR)
      {
         m_year = year;
      }
   }

   bool DateTime::fromTimeString(string str)
   {
      bool parseAccomplished = false;
      size_t endPos = 0;
      int16_t tempVal = 0;

      // NOTE: the expected date string format is YYYY-MM-DDThh:mm:ssZ

      endPos = parseTimeChunk(str, '-', 0, tempVal);
      if ((tempVal >= 0) &&
          (tempVal <= MAX_YEAR) &&
          (string::npos != endPos))
      {
         m_year = (uint16_t)tempVal;

         endPos = parseTimeChunk(str, '-', endPos + 1, tempVal);

         if ((tempVal >= 1) &&
             (tempVal <= MAX_MONTH) &&
             (string::npos != endPos))
         {
            m_month = (uint8_t)tempVal;

            endPos = parseTimeChunk(str, 'T', endPos + 1, tempVal);

            if ((tempVal >= 1) &&
                (tempVal <= MAX_DAY) &&
                (string::npos != endPos))
            {
               m_day = (uint8_t)tempVal;
               endPos = parseTimeChunk(str, ':', endPos + 1, tempVal);

               if ((tempVal >= 0) &&
                   (tempVal <= MAX_HOUR) &&
                   (string::npos != endPos))
               {
                  m_hour = (uint8_t)tempVal;
                  endPos = parseTimeChunk(str, ':', endPos + 1, tempVal);

                  if ((tempVal >= 0) &&
                      (tempVal <= MAX_MINUTE) &&
                      (string::npos != endPos))
                  {
                     m_minute = (uint8_t)tempVal;
                     endPos = parseTimeChunk(str, 'Z', endPos + 1, tempVal);

                     if ((tempVal >= 0) &&
                         (tempVal <= MAX_SECOND) &&
                         (string::npos != endPos))
                     {
                        m_second = (uint8_t)tempVal;
                        parseAccomplished = true;
                     }
                  }
               }
            }
         }
      }

      return parseAccomplished;
   }

   size_t DateTime::parseTimeChunk(const string& parse_str,
                                   const char seperator,
                                   const size_t startPos,
                                   int16_t& parsedValue)
   {
      parsedValue = -1;
      size_t endPos = string::npos;

      if (startPos <= parse_str.size())
      {
         endPos = parse_str.find(seperator, startPos);

         if ((string::npos != endPos) &&
             (endPos <= parse_str.size()) &&
             (startPos < endPos))
         {
            string digit_str = parse_str.substr(startPos, endPos - startPos);
            parsedValue = atoi(digit_str.c_str());
         }
      }

      return endPos;
   }

   string DateTime::toString()
   {
      ostringstream str;
      str << (int)getMonth() << "-" <<
             (int)getDay() << "-" <<
             (int)getYear() << " " <<

             (int)getHour() << ":" <<
             (int)getMinute() << ":" <<
             (int)getSecond();

      return str.str();
   }

   string DateTime::toUTCString()
   {
      char buffer [21];
      struct tm timeinfo;

      // build the time_t structure from this object's internal members
      timeinfo.tm_hour = m_hour;
      timeinfo.tm_min = m_minute;
      timeinfo.tm_sec = m_second;
      timeinfo.tm_mon = m_month - TM_MONTH_OFFSET;
      timeinfo.tm_mday = m_day;

      // NOTE: the struct tm adds 1900 to whatever value is contained in year.
      timeinfo.tm_year = m_year - TM_YEAR_OFFSET;

      // format the timeinfo string in the expected UTC format:
      // YYYY-MM-DDThh:mm:ssZ
      strftime (buffer, 21, "%FT%TZ", &timeinfo);

      string str(buffer);

      return str;
   }
}
