#ifndef DEBUGPRINT_H_
#define DEBUGPRINT_H_

namespace CecilStLabs
{
   /**
    * The feature category enumeration specifies the features category to enable
    *
    */
   enum eFeatureCategory
   {
      FEATURE_CAT_NONE  = 0x00,
      Unknown_1         = 0x01,
      Unknown_2         = 0x02,
      Unknown_3         = 0x04,
      Unknown_4         = 0x08,
      Unknown_5         = 0x10,
      Unknown_6         = 0x20,
      Unknown_7         = 0x40,
      Unknown_8         = 0x80,

      MAX_VALID_CAT     = 0xFF
   };

   /**
    * DebugPrint is used to create output that is sent to every "print appender"
    * that is added to this object.  This output is also filtered so specific "categories"
    * of objects producing print output can be turned on or off.  This allows
    * the output to be uncluttered if desired, or receive the entire picture of
    * operation by enabling all categories.
    */
   class DebugPrint
   {
      public:

         /**
          * Create the debug print object.
          *
          * @param clockDriver Driver to the clock to use for adding the date
          *                    and time to the messages.
          * @param currentCategory The category to print the message for.
          */
         DebugPrint(IClockDriver* clockDriver, eFeatureCategory currentCategory);

         /**
          * Release all of the resources
          */
         virtual ~DebugPrint();

         /**
          * Sets the current feature category.
          *
          * @param category The feature category to create
          */
         void setFeatureCategory(eFeatureCategory category);

         /**
          * Adds the specified appender to this debug print class.
          *
          * @param appender
          */
         void addLogAppender(IAppender* appender);

         /**
          * Prints the specified message to the loggers previously added.
          *
          * @param message The message to send to the loggers.
          * @param sourceCategory The category enumeration where the message is being
          *        printed from.
          */
         void print(std::string message, eFeatureCategory sourceCategory);

      protected:

         /**
          * The semaphore used to make sure the output doesn't get "stomped on"
          * by multiple sources attempting to print at the same time.
          */
         pthread_mutex_t m_printSem;

         /**
          * The mask used to filter debug messages to print.  If a feature's
          * Print Mask is set, the message will be print.
          */
         eFeatureCategory m_currFeatureCategory;

         /**
          * The debug output is needed to print the timestamp on each the debug
          * outputs, the clock driver is required to get the current time.
          */
         IClockDriver* m_clockDriver;

         /**
          * The constant maximum number of appenders statically allocated in
          * the appender array.
          */
         static const uint8_t MAX_NUM_APPENDERS = 1;

         /**
          * The current index into the array of appenders for adding new items.
          */
         uint8_t m_appenderIndex;

         /**
          * statically allocated list of appenders.
          */
         IAppender* m_appenders[MAX_NUM_APPENDERS];
   };
}

#endif
