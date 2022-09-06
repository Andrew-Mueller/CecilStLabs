#include <stdio.h>
#include <sys/socket.h>
#include <resolv.h>
#include <netdb.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#include <pthread.h>                            //lint !e537

#include <string>
#include <sstream>

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

#include "../../common/logging/LogEntry.h"
#include "../../common/util/IFileMonitor.h"
#include "../../common/util/IFileMonitorHandler.h"

#include "../../common/logging/ILogDriver.h"
#include "../../common/logging/emptyLogDriver.h"
#include "../../common/logging/ILoggable.h"

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

#include "SysLogMonitorThread.h"

namespace CecilStLabs
{

   SysLogMonitorThread::SysLogMonitorThread(LogEntryEvent& logEntryEvent,
                                            IFileMonitor* fileMonitor,
                                            ILogDriver* logDriver)
      :
        PeriodicThread(PERIODIC_TIMEOUT_SECONDS, ePeriodInterval_Seconds),
        m_logEntryEvent(logEntryEvent),
        m_syslogMonitor(fileMonitor),
        m_logDriver(logDriver)
   {
      // intentionally left blank.
   }

   SysLogMonitorThread::~SysLogMonitorThread()
   {
      Stop();
   }

   void SysLogMonitorThread::Start()
   {
      m_syslogMonitor->initialize();

      // handle all other startup actions needed by the parent.
      PeriodicThread::Start();
   }

   void SysLogMonitorThread::Stop()
   {
      // The PeriodicThread method blocks until the working function is closed.
      // This allows us to ensure we won't be waiting for any syslog message
      // using poll() or read() when we close it.
      PeriodicThread::Stop();

      // tear down the file monitoring object.
      m_syslogMonitor->destroy();
   }

   void SysLogMonitorThread::PeriodicAsyncWorker()
   {
      m_syslogMonitor->monitor();
   }

   void SysLogMonitorThread::handle(uint8_t* bytesRead, uint16_t numBytesRead)
   {
      // NOTE: The data passed is a single line from the SysLog.
      // don't do anything if the length is 0
      if (0 < numBytesRead)
      {
         std::string message((const char*)bytesRead);

         // parse the message as an event log message.
         LogEntry logEntry = m_logDriver->Parse(message);

         if (LoggingOFF != logEntry.getLogLevel())
         {
             // post the newly parsed message to jetstream.
             m_logEntryEvent.SetLogEntry(logEntry);
             m_logEntryEvent.Post();
         }
      }
   }
}
