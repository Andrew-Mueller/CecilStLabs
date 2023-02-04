#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//lint -esym(18,*)
#include <iostream>
#include <sstream>
using namespace std;

#include <sys/socket.h>
#include <resolv.h>
#include <netdb.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#include <pthread.h>                            //lint !e537

#include "../../common/tersoTypes.h"
#include "../../common/util/EnumByName.h"
#include "../../common/DateTime/DateTime.h"
#include "../../common/util/TersoUtil.h"
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

#include "HeartbeatThread.h"

// NOTE: this define should go away in favor of using the Debug Print Mask
#define HEARTBEATTHREAD_DEBUG_PRINT

namespace Terso
{
   HeartbeatThread::HeartbeatThread(std::string deviceAccessKey,
                                    std::string deviceSerialNumber,
                                    CommQueue* commQueue,
                                    IClockDriver* clockDriver,
                                    uint16_t hearbeatPeriod_minutes)

      : PeriodicThread(hearbeatPeriod_minutes, ePeriodInterval_Minutes),
        m_deviceAccessKey(deviceAccessKey),
        m_deviceSerialNumber(deviceSerialNumber),
        m_commQueue(commQueue),
        m_clockDriver(clockDriver),
        m_heartbeatURL(""),
        m_heartbeatBody("")
   {
      // build up the URL of the heartbeat REST interface point
      ostringstream urlBuilder;
      urlBuilder << "https://"
                 << m_commQueue->getEndpointURL()
                 << "/v1.0/device/?AccessKey="
                 << deviceAccessKey;

      m_heartbeatURL = urlBuilder.str();
   }

   HeartbeatThread::~HeartbeatThread()
   {
      // intentionally left blank.
   }

   void HeartbeatThread::PeriodicAsyncWorker()
   {
      sendHeartbeatNow();
   }

   void HeartbeatThread::BuildHeartbeatBody()
   {
      DateTime currTime = m_clockDriver->getCurrentTime();

      // build up the heartbeat XML Body
      ostringstream xmlBuilder;
      xmlBuilder << "<RestfulAPI xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\" "
                 << "xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" "
                 << "xmlns=\"http://RestfulAPI.CecilStLabs.com/v1.0/Device/HeartbeatEvent\"><Header EventTime=\"";

      xmlBuilder << currTime.toUTCString();

      xmlBuilder << "\" DeviceSerialNumber=\"";
      xmlBuilder << m_deviceSerialNumber;
      xmlBuilder << "\" /><HeartbeatEvent /></RestfulAPI>";

      m_heartbeatBody = xmlBuilder.str();
   }

   void HeartbeatThread::sendHeartbeatNow()
   {
      #ifdef HEARTBEATTHREAD_DEBUG_PRINT
         getLogDriver()->Log("Sending a Heartbeat...", LoggingDebug);
      #endif

      // make sure the current data is in the body of the message (time)
      BuildHeartbeatBody();

      // create the communication record for the heartbeat.
      CommRecord record(POST,
                        m_heartbeatURL,
                        m_heartbeatBody,
                        eHeartbeat_Msg,
                        m_clockDriver->getCurrentTime());

      // add the heartbeat to the communication queue.
      m_commQueue->enqueue(record);
   }
}
