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

#include "../../common/tersoTypes.h"
#include "../../common/util/EnumByName.h"
#include "../../common/DateTime/DateTime.h"
#include "../../common/util/TersoUtil.h"
#include "../../common/guid.h"

#include "../../common/logging/LogEntry.h"
#include "../../common/logging/ILogDriver.h"
#include "../../common/logging/emptyLogDriver.h"
#include "../../common/logging/ILoggable.h"

#include "../../common/LinkedList/SingleLink.h"
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
#include "../../common/util/NameValuePair.h"
#include "../../common/InternalEvent/IInternalEvent.h"
#include "../../common/InternalEvent/IInternalEventHandler.h"
#include "../../common/InternalEvent/InternalEventRegistry.h"

#include "../Events/jetstreamEvent.h"
#include "../Events/CommandCompletionEvent.h"

#include "../Commands/Command.h"
#include "../Commands/ResetCommand.h"
#include "../Commands/GetConfigValuesCommand.h"
#include "../Commands/SetConfigValuesCommand.h"
#include "../Commands/JetstreamCommands.h"
#include "../Commands/CommandList.h"

#include "../jetstreamMessages.h"
#include "../jetstreamMessageParser.h"

#include "../../comms/CommStateMachine/CommsSignals.h"
#include "../../comms/CommStateMachine/BidirectionalCommsAvailableState.h"
#include "../../comms/CommStateMachine/UnidirectionalCommsAvailableState.h"
#include "../../comms/CommStateMachine/CommsUnavailableState.h"

#include "../../comms/CommQueue.h"

#include "jetstreamEvent.h"
#include "LogEntryEvent.h"

namespace Terso
{

   LogEntryEvent::LogEntryEvent(std::string deviceAccessKey,
                                std::string deviceSerialNumber,
                                IClockDriver* clockDriver,
                                CommQueue* commQueue,
                                LogEntry& logEntry)
      : jetstreamEvent(deviceAccessKey,
                       deviceSerialNumber,
                       clockDriver,
                       eLogEntryEvent_Msg),
        m_logEntry(logEntry)
   {
      setCommunicationQueue(commQueue);
   }

   LogEntryEvent::~LogEntryEvent()
   {
      // intentionally left blank
   }

   void LogEntryEvent::SetLogEntry(LogEntry& logEntry)
   {
      m_logEntry = logEntry;
   }

   std::string LogEntryEvent::buildXML()
   {
      DateTime currentTime = m_clockDriver->getCurrentTime();
      ostringstream xml;

      xml << "<?xml version=\"1.0\"?>";
      xml << "<Jetstream xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\" ";
      xml << "xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" ";
      xml << "xmlns=\"http://Jetstream.TersoSolutions.com/v1.0/Device/LogEntryEvent\">";
      xml << "<Header DeviceSerialNumber=\"" << m_logicalDeviceID;
      xml << "\" EventTime=\"" << currentTime.toUTCString() << "\" />";

      xml << "<LogEntryEvent>";
      xml <<   "<LogEntryList>";
      xml <<      "<LogEntry Level=\"" << m_logEntry.getLogLevelName() << "\" ";
      xml <<                "Message=\"" << m_logEntry.getMessage() << "\" ";
      xml <<                "Type=\"" << m_logEntry.getType() << "\" ";
      xml <<                "LogTime=\"" << m_logEntry.getTimeStamp().toUTCString() << "\">";
      xml <<         "<ParameterList />";
      xml <<      "</LogEntry>";
      xml <<   "</LogEntryList>";
      xml << "</LogEntryEvent>";
      xml << "</Jetstream>";

      return xml.str();
   }
}
