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

#include "../../common/hardware/sensor/ISensor.h"
#include "../../common/hardware/sensor/ISensorConverted.h"
#include "../../common/hardware/sensor/SensorReading.h"

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

#include "../comms/MessageParser.h"

#include "../../comms/CommStateMachine/CommsSignals.h"
#include "../../comms/CommStateMachine/BidirectionalCommsAvailableState.h"
#include "../../comms/CommStateMachine/UnidirectionalCommsAvailableState.h"
#include "../../comms/CommStateMachine/CommsUnavailableState.h"

#include "../../comms/CommQueue.h"

#include "SensorReadingThread.h"

namespace Terso
{

   SensorReadingThread::SensorReadingThread(std::string deviceAccessKey,
                                            std::string deviceSerialNumber,
                                            CommQueue* commQueue,
                                            IClockDriver* clockDriver,
                                            uint16_t sensorReadingPeriod_minutes)
      : PeriodicThread(sensorReadingPeriod_minutes, ePeriodInterval_Minutes),
        m_inTemperatureAlarm(false),
        m_deviceAccessKey(deviceAccessKey),
        m_deviceSerialNumber(deviceSerialNumber),
        m_commQueue(commQueue),
        m_clockDriver(clockDriver),
        m_sensorReadingEvent(deviceAccessKey,
                             deviceSerialNumber,
                             clockDriver)
   {
      // build up the Sensor Reading Event URL
      ostringstream urlBuilder;

      urlBuilder << "https://"
                 << m_commQueue->getJetstreamURL()
                 << "/v1.0/device/?AccessKey="
                 << deviceAccessKey;

      m_sensorReadingURL = urlBuilder.str();
   }

   SensorReadingThread::~SensorReadingThread()
   {
      // intentionally left blank.
   }

   void SensorReadingThread::addSensor(ISensor* sensor)
   {
      if (NULL != sensor)
      {
         m_sensorReadingEvent.addSensor(sensor);
      }
   }

   void SensorReadingThread::SetPeriod(const uint16_t period, const ePeriodInterval periodInterval)
   {
      if (!m_inTemperatureAlarm)
      {
         PeriodicThread::SetPeriod(period, periodInterval);
      }
   }

   void SensorReadingThread::PeriodicAsyncWorker()
   {
      // read all of the sensors from the Sensor Reading Event.
      // NOTE: its important to call this before generating the XML, otherwise
      //       there will be no readings in the xml body.
      m_sensorReadingEvent.readSensors();

      getLogDriver()->Log("Sending a sensor reading event...", LoggingDebug);

      // make sure the current data is in the body of the message
      m_sensorReadingBody = m_sensorReadingEvent.getXML();

      // create the communication record for the sensor reading event.
      CommRecord record(POST,
                        m_sensorReadingURL,
                        m_sensorReadingBody,
                        eSensorReadingEvent_Msg,
                        m_clockDriver->getCurrentTime());

      // add the sensor reading event to the communication queue.
      m_commQueue->enqueue(record);
   }
}
