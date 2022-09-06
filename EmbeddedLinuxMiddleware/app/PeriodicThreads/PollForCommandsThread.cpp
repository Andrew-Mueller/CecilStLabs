
#include <cstdio>
#include <cstdlib>
#include <cstring>

//lint -esym(18,*)
#include <iostream>
#include <sstream>

#include <sys/socket.h>
#include <resolv.h>
#include <netdb.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#include <pthread.h>                            //lint !e537


#include "../../common/basicTypes.h"
#include "../../common/util/EnumByName.h"
#include "../../common/DateTime/DateTime.h"
#include "../../common/util/BasicUtil.h"
#include "../../common/guid.h"

#include "../../common/logging/LogEntry.h"
#include "../../common/logging/ILogDriver.h"
#include "../../common/logging/emptyLogDriver.h"
#include "../../common/logging/ILoggable.h"

#include "../../common/RingBuffer/RingBuffer.h"
#include "../../common/MemoryPool/MemoryPool.h"
#include "../../common/threads/PeriodicThread.h"

#include "../../common/DateTime/IClockDriver.h"

#include "../../comms/RXData/RXData.h"
#include "../../comms/SocketError.h"
#include "../../comms/SocketBase.h"
#include "../../comms/Socket.h"
#include "../../comms/SecureSession.h"
#include "../../comms/SecureSocket.h"

#include "../../comms/ICommProtocolResponse.h"
#include "../../comms/ICommProtocol.h"
#include "../../comms/HTTP/HTTPResponse.h"
#include "../../comms/HTTP/HTTPS.h"

#include "../../common/stateMachine/IState.h"
#include "../../common/stateMachine/Transition.h"
#include "../../common/stateMachine/ITransitionHandler.h"
#include "../../common/stateMachine/State.h"
#include "../../common/stateMachine/StateMachine.h"

#include "../../comms/CommRecord.h"
#include "../../comms/CommDAL.h"

#include "../../common/tinyxml/tinyxml2.h"
using namespace tinyxml2;

#include "../../common/config/IConfig.h"
#include "../../common/config/NullConfig.h"

#include "../../common/util/NameValuePair.h"
#include "../../common/InternalEvent/IInternalEvent.h"
#include "../../common/InternalEvent/IInternalEventHandler.h"
#include "../../common/InternalEvent/InternalEventRegistry.h"

#include "../../comms/CommStateMachine/MessageParser.h"

#include "../../comms/CommStateMachine/CommsSignals.h"
#include "../../comms/CommStateMachine/BidirectionalCommsAvailableState.h"
#include "../../comms/CommStateMachine/UnidirectionalCommsAvailableState.h"
#include "../../comms/CommStateMachine/CommsUnavailableState.h"

#include "../../comms/CommQueue.h"

#include "PollForCommandsThread.h"

#define POLLFORCMDS_DEBUG_PRINT

namespace CecilStLabs
{
   PollForCommandsThread::PollForCommandsThread(std::string deviceAccessKey,
                                                CommQueue& commQueue,
                                                uint16_t pollCommand_minutes)
      : PeriodicThread(pollCommand_minutes, ePeriodInterval_Minutes),
        m_commQueue(commQueue),
        m_getCommandsURL(""),
        m_deviceAccessKey(deviceAccessKey)
   {
      // build up the Poll for Commands URL
      std::ostringstream urlBuilder;
      urlBuilder << "https://" << m_commQueue.getJetstreamURL()
                 << "/v1.0/device/GetCommands?AccessKey="
                 << m_deviceAccessKey;

      m_getCommandsURL = urlBuilder.str();
   }

   PollForCommandsThread::~PollForCommandsThread()
   {
      // intentionally left blank
   }

   void PollForCommandsThread::PeriodicAsyncWorker()
   {
      getLogDriver()->Log("Getting Queued Commands", LoggingDebug, std::string(""));

      CommRecord record(POST,
                        m_getCommandsURL,
                        "",
                        ePollForCommands_Msg,
                        DateTime());

      m_commQueue.enqueue(record);
   }
}
