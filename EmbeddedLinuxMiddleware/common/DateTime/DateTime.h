#ifndef DATETIME_H
#define DATETIME_H

namespace CecilStLabs
{
   enum eMonthNames
   {
      January     = 1,
      February    = 2,
      March       = 3,
      April       = 4,
      May         = 5,
      June        = 6,
      July        = 7,
      August      = 8,
      September   = 9,
      October     = 10,
      November    = 11,
      December    = 12,

      MAX_MONTHS = December
   };

   static const EnumByName monthFullNames[] =
   {
         EnumByName((uint32_t)January,    "January"),
         EnumByName((uint32_t)February,   "February"),
         EnumByName((uint32_t)March,      "March"),
         EnumByName((uint32_t)April,      "April"),
         EnumByName((uint32_t)May,        "May"),
         EnumByName((uint32_t)June,       "June"),
         EnumByName((uint32_t)July,       "July"),
         EnumByName((uint32_t)August,     "August"),
         EnumByName((uint32_t)September,  "September"),
         EnumByName((uint32_t)October,    "October"),
         EnumByName((uint32_t)November,   "November"),
         EnumByName((uint32_t)December,   "December")
   };

   static const EnumByName monthAbbrNames[] =
   {
         EnumByName((uint32_t)January,    "Jan"),
         EnumByName((uint32_t)February,   "Feb"),
         EnumByName((uint32_t)March,      "Mar"),
         EnumByName((uint32_t)April,      "Apr"),
         EnumByName((uint32_t)May,        "May"),
         EnumByName((uint32_t)June,       "Jun"),
         EnumByName((uint32_t)July,       "Jul"),
         EnumByName((uint32_t)August,     "Aug"),
         EnumByName((uint32_t)September,  "Sept"),
         EnumByName((uint32_t)October,    "Oct"),
         EnumByName((uint32_t)November,   "Nov"),
         EnumByName((uint32_t)December,   "Dec")
   };

   class DateTime
   {
      public:
         /**
          * Provide the time to set the system time to.
          */
         DateTime();

         /**
          * Release any resources used by the DateTime object.
          */
         virtual ~DateTime();

         /**
          * Value equality operator for checking if the value of two
          * datetime objects are equivalent.
          *
          * @param copy DateTime to compare against.
          * @return True if the two DateTimes are equivelent. False if they do not.
          */
         bool operator==(const DateTime& rhs) const;
         bool operator!=(const DateTime& rhs) const;

         /**
          * Assignment operator to copy the date and time
          *
          * @param rhs DateTime to copy the values from.
          *
          * @return DateTime containing the same hour, minute, second, day,
          *         month, and year as the DateTime specified.
          */
         DateTime& operator=(const DateTime& rhs);

         /**
          * Provide the time to set the system time to.
          *
          * @param hour The hour to set the time.
          * @param minute The minute to set the time.
          * @param second The second to set the time.
          * @param month The month to set the time (1 - 12).
          * @param day The day to set the time.
          * @param year The year to set the time.
          */
         void setTimeValue(uint8_t hour,
                           uint8_t minute,
                           uint8_t second,
                           uint8_t month,
                           uint8_t day,
                           uint16_t year);

         /**
          * Expected format YYYY-MM-DDThh:mm:ssZ
          *
          * @parse date_str The Date string to parse into this date object
          *
          * @return True if the string was parsed successfully, False if an
          *         error occurred.
          */
         bool fromTimeString(std::string date_str);

         /**
          * Access-ers for the parts that makeup the date and time object.
          */
         inline uint8_t getHour() { return m_hour; };
         inline uint8_t getMinute() { return m_minute; };
         inline uint8_t getSecond() { return m_second; };
         inline uint8_t getMonth() { return m_month; };
         inline uint8_t getDay() { return m_day; };
         inline uint16_t getYear() { return m_year; };

         /**
          * returns the string representation of the date.
          */
         std::string toString();

         /**
          * Builds the string in the UTC format yyyy-mm-ddTHH:mm:ssZ
          */
         std::string toUTCString();

      protected:

         // NOTE: only mins are specified if they are non-zero, otherwise the
         //       unsigned data type implicitly ensures the minimum is greater
         //       than zero.
         static const uint8_t MAX_HOUR = 23;
         static const uint8_t MAX_MINUTE = 59;
         static const uint8_t MAX_SECOND = 59;
         static const uint8_t MIN_MONTH = 1;
         static const uint8_t MAX_MONTH = 12;
         static const uint8_t MIN_DAY = 1;
         static const uint8_t MAX_DAY = 31;
         static const uint16_t MAX_YEAR = 9999;

         // NOTE: the tm structure used for formatting the UTC string adds the
         //       following constant to the year.
         static const uint16_t TM_YEAR_OFFSET = 1900;

         // NOTE: the tm structure used for formatting the UTC string subtracts
         //       1 from the month value because we use 1-12 but tm uses 0-11.
         static const uint8_t TM_MONTH_OFFSET = 1;

         /**
          * Hour value 0 - 23
          */
         uint8_t m_hour;

         /**
          * minute value 0 - 59
          */
         uint8_t m_minute;

         /**
          * second value 0 - 59
          */
         uint8_t m_second;

         /**
          * Month value 1 - 12
          */
         uint8_t m_month;

         /**
          * Day value, 1 - 31
          */
         uint8_t m_day;

         /**
          * 4 digit year value, 0 - 9999
          * NOTE: no offset like 1900 or 1970 which is system specific.
          */
         uint16_t m_year;

         /**
          * Parse a chunk of the Date string.  Parts of the date string is
          * separated by a single character.  The date string is in the format
          * YYYY-MM-DDThh:mm:ssZ
          *
          * @param parse_str The original full string for parsing.
          * @param separator The character that marks the end of the desired substring to parse.
          * @param startPos The position to start looking for the substring from.
          * @param parsedValue OUT The value parsed from the string.  Negative if an error occurred.
          * @return Ending position of the substring parsed.
          */
         size_t parseTimeChunk(const std::string& parse_str,
                               const char separator,
                               const size_t startPos,
                               int16_t& parsedValue);
   };
}
#endif
