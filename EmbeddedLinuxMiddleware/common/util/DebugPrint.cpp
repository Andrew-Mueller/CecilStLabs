#include <cstring>

//lint -esym(18,*)
#include <iostream>
#include <sstream>
#include <iostream>
#include <ostream>
using namespace std;

#include <pthread.h>                            //lint !e537

#include "../basicTypes.h"
#include "../util/EnumByName.h"
#include "../DateTime/DateTime.h"


#include "../DateTime/IClockDriver.h"
#include "../logging/LogEntry.h"
#include "../logging/ILogDriver.h"

#include "BasicUtil.h"

#include "IAppender.h"
#include "DebugPrint.h"

namespace CecilStLabs
{

   DebugPrint::DebugPrint(IClockDriver* clockDriver, eFeatureCategory currentCategory)
      : m_printSem(),
        m_currFeatureCategory(currentCategory),
        m_clockDriver(clockDriver),
        m_appenderIndex(0)

   {
      // Initialize the appender semaphore.
      // Halt the application if the semaphore cannot be created.
      BasicUtil::Assert((0 == pthread_mutex_init(&m_printSem, NULL)), __FILE__, __LINE__);
   }

   DebugPrint::~DebugPrint()
   {
      // TODO Auto-generated destructor stub
   }

   void DebugPrint::setFeatureCategory(eFeatureCategory category)
   {
      // TODO: forcing the parameter to the feature category enumeration should
      //       offer some value protection... but this feels dangerous

      // the enumeration type should offer some protection for the range, but
      // a bad value could sneak in.  Be sure it is within range before assuming
      // it is good.
      int verifyCat = (int8_t)category;

      // verify the specified new category is valid
      if ((MAX_VALID_CAT >= verifyCat) &&
          (0 >= verifyCat))
      {
         // TODO: throw exception?
      }
      else
      {
         m_currFeatureCategory = category;
      }
   }

   void DebugPrint::addLogAppender(IAppender* appender)
   {
      // verify their is still room in the array of appenders.
      BasicUtil::Assert((m_appenderIndex < MAX_NUM_APPENDERS), __FILE__, __LINE__);

      // add the specified appender to the array.
      m_appenders[m_appenderIndex++] = appender;
   }

   void DebugPrint::print(std::string message, eFeatureCategory sourceCategory)
   {
      if (MUTEX_SUCCESS == pthread_mutex_lock(&m_printSem))
      {
         // check if the current is category
         if ((sourceCategory && m_currFeatureCategory) > 0)
         {
            for (uint8_t i = 0; i < m_appenderIndex; i++)
            {
               // append the message to each of the appenders.
               m_appenders[i]->Append(message);
            }
         }

         pthread_mutex_unlock(&m_printSem);
      }
   }
}
