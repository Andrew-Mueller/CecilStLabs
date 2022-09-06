#include "../../common/basicTypes.h"
#include "../../common/util/EnumByName.h"
#include "../../common/DateTime/DateTime.h"
#include "../../common/util/BasicUtil.h"
#include "../../common/guid.h"

#include "../../common/logging/LogEntry.h"
#include "../../common/logging/ILogDriver.h"
#include "../../common/logging/emptyLogDriver.h"
#include "../../common/logging/ILoggable.h"

#include "../../common/DateTime/IClockDriver.h"

#include "../../common/tinyxml/tinyxml2.h"
using namespace tinyxml2;

#include "../../common/config/IConfig.h"
#include "../../common/config/NullConfig.h"

#include "CommandList.h"
#include "MessageParser.h"

namespace CecilStLabs
{
    MessageParser::MessageParser(CommandList* commandList,
                                 IClockDriver* clockDriver,
                                 IConfig& wholeConfig,
                                 std::string accessKey,
                                 std::string logicalDeviceID)
    {
        // intentionally left blank
    }

    void MessageParser::parse(uint32_t id, std::string message)
    {
        switch(id)
        {
            default :
            {
                getLogDriver()->Log("Message Not Expected", LoggingError, "");

                // unexpected message id type, hault the application
                BasicUtil::Assert(false, __FILE__, __LINE__);

                break;
            }
        }
    }
}
