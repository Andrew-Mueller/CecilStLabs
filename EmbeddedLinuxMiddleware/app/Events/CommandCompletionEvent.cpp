#include <iostream>
#include <sstream>
using namespace std;

#include <sys/socket.h>
#include <resolv.h>
#include <netdb.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#include "../../common/basicTypes.h"
#include "../../common/util/BasicUtil.h"
#include "../../common/util/EnumByName.h"

#include "../../common/guid.h"
#include "../../common/DateTime/DateTime.h"

#include "../../common/logging/LogEntry.h"
#include "../../common/logging/ILogDriver.h"
#include "../../common/logging/emptyLogDriver.h"
#include "../../common/logging/ILoggable.h"

#include "../../common/LinkedList/LinkedListCommon.h"
#include "../../common/LinkedList/SingleLink.h"
#include "../../common/LinkedList/SingleLinkedList.h"

#include "../../common/RingBuffer/RingBuffer.h"
#include "../../common/MemoryPool/MemoryPool.h"

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

#include "../../common/logging/LogEntry.h"
#include "../../common/logging/ILogDriver.h"
#include "../../common/logging/emptyLogDriver.h"
#include "../../common/logging/ILoggable.h"

#include "../../common/config/IConfig.h"
#include "../../common/util/NameValuePair.h"
#include "../../common/InternalEvent/IInternalEvent.h"
#include "../../common/InternalEvent/IInternalEventHandler.h"
#include "../../common/InternalEvent/InternalEventRegistry.h"

#include "Event.h"
#include "CommandCompletionEvent.h"

#include "../Commands/Command.h"
#include "../Commands/ResetCommand.h"
#include "../Commands/GetConfigValuesCommand.h"
#include "../Commands/SetConfigValuesCommand.h"
#include "../Commands/Commands.h"
#include "../Commands/CommandList.h"

#include "../Messages.h"
#include "../MessageParser.h"

#include "../../comms/CommStateMachine/CommsSignals.h"
#include "../../comms/CommStateMachine/BidirectionalCommsAvailableState.h"
#include "../../comms/CommStateMachine/UnidirectionalCommsAvailableState.h"
#include "../../comms/CommStateMachine/CommsUnavailableState.h"
#include "../../comms/CommQueue.h"

namespace Terso
{
   CommandCompletionEvent::CommandCompletionEvent(std::string deviceAccessKey,
                                                  std::string deviceSerialNumber,
                                                  IClockDriver* clockDriver)
      : jetstreamEvent(deviceAccessKey,
                       deviceSerialNumber,
                       clockDriver,
                       eCommandCompletion_Msg),
              m_parameterList(),
              m_parameterPool(m_parameters, MAX_PARAMETER_ITEMS)

   {
      // clear out the parameter list
      for (uint16_t i = 0; i < MAX_PARAMETER_ITEMS; i++)
      {
         m_parameters[i].setName(string(""));
         m_parameters[i].setValue(string(""));
      }
   }

   CommandCompletionEvent::~CommandCompletionEvent()
   {
      // return all of the parameters to the memory pool and remove all of them
      // from the linked list.
      clear();
   }

   void CommandCompletionEvent::addOutputParameter(std::string name, std::string value)
   {
      NameValuePair* param = (NameValuePair*)m_parameterPool.allocate();

      if (NULL != param)
      {
         param->setName(name);
         param->setValue(value);

         m_parameterList.insert((SingleLink*)param);
      }
      else
      {
         this->getLogDriver()->log("parameter pool is empty.", LoggingDebug, "");
      }
   }

   void CommandCompletionEvent::clear()
   {
      // iterate over the linked list of parameters, returning each to the memory pool.
      NameValuePair* param = (NameValuePair*)m_parameterList.first();

      while (NULL != param)
      {
         // remove the parameter from the linked list.
         param = (NameValuePair*)m_parameterList.remove(param);

         if (NULL == param)
         {
            this->getLogDriver()->log("Error removing the parameter from the linked list.", LoggingDebug, "");
         }
         else
         {
            // return the parameter back to the memory pool.
            m_parameterPool.free(param);
         }

         // NOTE: we always use first() on every iteration to "remove from the head".  This ensures
         //       that every element in the list gets hit and we don't accidentally skip over some
         //       by using next() instead.
         param = (NameValuePair*)m_parameterList.first();
      }
   }

   std::string CommandCompletionEvent::buildXML()
   {
      DateTime currentTime = m_clockDriver->getCurrentTime();
      ostringstream xml;

      xml << "<?xml version=\"1.0\"?>";
      xml << "<Jetstream xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\" ";
      xml << "xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" ";
      xml << "xmlns=\"http://Jetstream.TersoSolutions.com/v1.0/Device/CommandCompletionEvent\">";
      xml << "<Header EventTime=\"" << currentTime.toUTCString() << "\" />";
      xml << " <CommandCompletionEvent CommandId=\"";
      xml << m_commandID.ToString();
      xml << "\">";

      if (0 == m_parameterList.length())
      {
         xml << "<OutputParameterList/>";
      }
      else
      {
         xml << "<OutputParameterList>";

         for (NameValuePair* param = (NameValuePair*)m_parameterList.first();
              param != NULL;
              param = (NameValuePair*)m_parameterList.next(param))
         {
            xml << "<OutputParameter Name=\"" << param->getName() << "\" Value=\"" << param->getValue() << "\"/>";
         }

         xml << "</OutputParameterList>";
      }

      // TODO: exception list and device extension list is unimplemented.

      xml << "<ExceptionList/>";
      xml << "<DeviceExtensionList />";
      xml << "</CommandCompletionEvent>";

      xml << "</Jetstream>";

      return xml.str();
   }
}
