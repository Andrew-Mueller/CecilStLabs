#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>
using namespace std;

#include "../basicTypes.h"
#include "../util/EnumByName.h"
#include "../DateTime/DateTime.h"

#include "../logging/LogEntry.h"
#include "../logging/ILogDriver.h"
#include "../logging/emptyLogDriver.h"
#include "../logging/ILoggable.h"

#include "../util/BasicUtil.h"

#include "../DateTime/IClockDriver.h"

#include "../util/IAppender.h"
#include "../util/DebugPrint.h"

#include "../LinkedList/SingleLink.h"
#include "../RingBuffer/RingBuffer.h"
#include "../guid.h"

#include "../tinyxml/tinyxml2.h"
using namespace tinyxml2;

#include "IConfig.h"
#include "IConfigAction.h"
#include "ConfigActionRead.h"
#include "ConfigActionWrite.h"
#include "NetworkConfigSection.h"

namespace CecilStLabs
{
   // define all of the constant string values
   const std::string NetworkConfigSection::XMLTAG_NETWORK_CONFIG = "NetworkConfig";

   const std::string NetworkConfigSection::XMLATTR_VERSION = "version";
   const std::string NetworkConfigSection::XMLTAG_ACCESS_KEY = "AccessKey";
   const std::string NetworkConfigSection::XMLTAG_COMMS_RECONNECT_INTERVAL = "CommsReconnectInterval";
   const std::string NetworkConfigSection::XMLTAG_COMMAND_POLL_INTERVAL = "CommandPollInterval";
   const std::string NetworkConfigSection::XMLTAG_COMMS_QUEUE_SECONDS_INTERVAL = "CommunicationQueueSecondsInterval";
   const std::string NetworkConfigSection::XMLTAG_DEVICE_SERIAL_NUMBER = "DeviceSerialNumber";
   const std::string NetworkConfigSection::XMLTAG_DNS = "DNS";
   const std::string NetworkConfigSection::XMLTAG_GATEWAY = "Gateway";
   const std::string NetworkConfigSection::XMLTAG_IP = "IP";
   const std::string NetworkConfigSection::XMLTAG_SUBNET = "Subnet";
   const std::string NetworkConfigSection::XMLTAG_LOG_ENTRY_LEVEL = "LogEntryLevel";
   const std::string NetworkConfigSection::XMLTAG_QUEUE_FILENAME = "QueueFile";
   const std::string NetworkConfigSection::XMLTAG_RTCDEVICE_PATH = "RTCDevicePath";
   const std::string NetworkConfigSection::XMLTAG_DEBUG_PRINT_MASK = "DebugPrintMask";

   const std::string NetworkConfigSection::DEFAULT_QUEUE_FILENAME = "queue";
   const std::string NetworkConfigSection::DEFAULT_RTC_DEVICE_PATH = "/dev/rtc";

   NetworkConfigSection::NetworkConfigSection()
      : m_version(1),
        m_accessKey(),
        m_commsReconnectInterval_seconds(DEFAULT_COMMS_RECONNECT_INTERVAL_SECONDS),
        m_commandPollInterval(0),
        m_commQueueSecondsInterval(DEFAULT_COMMUNICATION_QUEUE_SECONDS_INTERVAL),
        m_deviceSerialNumber(""),
        m_DNS("0.0.0.0"),
        m_gateway("0.0.0.0"),
        m_ip("127.0.0.1"),
        m_subnet("255.255.255.0"),
        m_logEntryLevel(LoggingDebug),
        m_queueFilename(DEFAULT_QUEUE_FILENAME),
        m_rtcDevicePath(DEFAULT_RTC_DEVICE_PATH),
        m_debugPrintMask(DEFAULT_DEBUG_PRINT_MASK)
   {
      // intentionally left blank.
   }

   NetworkConfigSection::~NetworkConfigSection()
   {
      // intentionally left blank.
   }

   bool NetworkConfigSection::readSection(std::string configStr)
   {
      bool success = false;

      IConfigAction* configReader = new ConfigActionRead();

      // Expecting configStr to start at the <ExampleConfig> tag
      // TODO: verify that this tag name is in fact the one specified?

      XMLDocument xmlDoc;
      XMLError parseStatus = xmlDoc.Parse(configStr.c_str());

      if (XML_NO_ERROR == parseStatus)
      {
         // the root element should be "NetworkConfig"
         XMLElement* root = xmlDoc.RootElement();
         stringstream buffer;

         // re-usable unsigned int value for converting to the correct data type;
         unsigned int tempReadUInt = 0;

         // get the version number of the config section and convert it to an integer.
         const char* version = root->Attribute(XMLATTR_VERSION.c_str());
         if (NULL != version)
         {
            string versionStr(version);

            if (!versionStr.empty())
            {
               tempReadUInt = 0;

               buffer.str(versionStr);
               buffer >> tempReadUInt;
               m_version = tempReadUInt;
            }

            // TODO: in the future, use the version number just read to parse with
            //       the appropriate AppVersion Object

            configReader->execute(root, XMLTAG_ACCESS_KEY.c_str(), buffer);
            if( 0 < buffer.str().size() )
            {
               string accessKeyStr;
               buffer >> accessKeyStr;
               m_accessKey.Set(accessKeyStr);
            }

            configReader->execute(root, XMLTAG_COMMS_RECONNECT_INTERVAL.c_str(), buffer);
            if( 0 < buffer.str().size() )
            {
               tempReadUInt = 0;
               buffer >>  tempReadUInt;
               m_commsReconnectInterval_seconds = tempReadUInt;
            }

            configReader->execute(root, XMLTAG_COMMAND_POLL_INTERVAL.c_str(), buffer);
            if( 0 < buffer.str().size() )
            {
               tempReadUInt = 0;
               buffer >> tempReadUInt;
               m_commandPollInterval = tempReadUInt;
            }

            configReader->execute(root, XMLTAG_COMMS_QUEUE_SECONDS_INTERVAL.c_str(), buffer);
            if( 0 < buffer.str().size() )
            {
               tempReadUInt = 0;
               buffer >> m_commQueueSecondsInterval;
               m_commQueueSecondsInterval = tempReadUInt;
            }

            configReader->execute(root, XMLTAG_DEVICE_SERIAL_NUMBER.c_str(), buffer);
            if( 0 < buffer.str().size() )
            {
               buffer >> m_deviceSerialNumber;
            }

            configReader->execute(root, XMLTAG_DNS.c_str(), buffer);
            if( 0 < buffer.str().size() )
            {
               buffer >> m_DNS;
            }

            configReader->execute(root, XMLTAG_GATEWAY.c_str(), buffer);
            if( 0 < buffer.str().size() )
            {
               buffer >> m_gateway;
            }

            configReader->execute(root, XMLTAG_IP.c_str(), buffer);
            if( 0 < buffer.str().size() )
            {
               buffer >> m_ip;
            }

            configReader->execute(root, XMLTAG_SUBNET.c_str(), buffer);
            if( 0 < buffer.str().size() )
            {
               buffer >> m_subnet;
            }

            // read the LogEntry Level
            // TODO: what to do if the log entry level does not match any of the
            //       existing levels?  Set to default?
            configReader->execute(root, XMLTAG_LOG_ENTRY_LEVEL.c_str(), buffer);
            if( 0 < buffer.str().size() )
            {
               for (int i = 0; i < MAX_LOG_LEVELS; i++)
               {
                  if (buffer.str() == LogEntry::LOGLEVEL_NAMES[i].getName())
                  {
                     m_logEntryLevel = (eLogLevel)LogEntry::LOGLEVEL_NAMES[i].getType();
                  }
               }
            }

            configReader->execute(root, XMLTAG_QUEUE_FILENAME.c_str(), buffer);
            if( 0 < buffer.str().size() )
            {
               buffer >> m_queueFilename;
            }

            configReader->execute(root, XMLTAG_RTCDEVICE_PATH.c_str(), buffer);
            if( 0 < buffer.str().size() )
            {
               buffer >> m_rtcDevicePath;
            }

            configReader->execute(root, XMLTAG_DEBUG_PRINT_MASK.c_str(), buffer);
            if( 0 < buffer.str().size() )
            {
               string tempDebugPrint;
               buffer >> tempDebugPrint;


               // NOTE: the hex representation of the DebugPrintMask here requires
               //       a little stringstream magic below with the use of the temp string.
               stringstream streamBuff;
               streamBuff << std::hex << tempDebugPrint.c_str();
               streamBuff >> tempReadUInt;

               m_debugPrintMask = tempReadUInt;
            }

            success = true;
         }
      }

      // release the resource that we created at the top of this method.
      if (NULL != configReader)
      {
         delete configReader;
         configReader = NULL;
      }

      return success;
   }

   std::string NetworkConfigSection::writeSection(XMLNode& xmlParentNode)
   {
      // look into tinyxml2's memory management for the usecase below (creating
      // elements from the static document, etc.)

      // default is 4 spaces used for indenting the xml and a single new line for
      // a line break.
      // These can be changed at the end of the function using the
      // XMLPrinter::SetIndent and SetLineBreak methods.

      // the xml document used for allocating (and presumably manage) the
      // new SAX objects.
      XMLDocument* xmlDoc = NULL;

      // the element that is the immediate parent to all elements that will be
      // added here.
      XMLElement* xmlRootElem = NULL;

      // the primary element that all following configuration values will be added.
      XMLElement* exampleApp_xmlElem = NULL;

      // the specified node passed in as the argument is expected to be the root
      // element that we will add this configuration section into.
      xmlRootElem = xmlParentNode.ToElement();

      if (NULL != xmlRootElem)
      {
         xmlDoc = xmlRootElem->GetDocument();

         // create the xmlRootElem element for the configuration section.
         exampleApp_xmlElem = xmlDoc->NewElement(XMLTAG_NETWORK_CONFIG.c_str());
         xmlRootElem->LinkEndChild(exampleApp_xmlElem);
      }
      else
      {
         // the passed in node cannot be cast to an element.  it might be the document
         // root itself, so try that.
         xmlDoc = xmlParentNode.ToDocument();
         if (NULL != xmlDoc)
         {
            exampleApp_xmlElem = xmlDoc->NewElement(XMLTAG_NETWORK_CONFIG.c_str());
            xmlRootElem->LinkEndChild(exampleApp_xmlElem);
         }
         else
         {
            // these are the only two cases we care to deal with right now so
            // strap on your parachute and punch out.
            BasicUtil::Assert((NULL != xmlRootElem), __FILE__, __LINE__);
         }
      }

      // set the version number of the config section.
      stringstream buffer;
      buffer.clear();
      buffer.str("");
      buffer << std::setw(3) << std::setfill('0') << (int)m_version;
      exampleApp_xmlElem->SetAttribute(XMLATTR_VERSION.c_str(), buffer.str().c_str());

      XMLElement* accessKey_xmlElem = xmlDoc->NewElement(XMLTAG_ACCESS_KEY.c_str());
      XMLText* accessKey_val = xmlDoc->NewText(m_accessKey.ToString().c_str());
      accessKey_xmlElem->LinkEndChild(accessKey_val);
      exampleApp_xmlElem->LinkEndChild(accessKey_xmlElem);

      XMLElement* commsReconnect_xmlElem = xmlDoc->NewElement(XMLTAG_COMMS_RECONNECT_INTERVAL.c_str());
      buffer.clear();
      buffer.str("");
      buffer << (int)m_commsReconnectInterval_seconds;
      XMLText* commsReconnect_val = xmlDoc->NewText(std::string(buffer.str()).c_str());
      commsReconnect_xmlElem->LinkEndChild(commsReconnect_val);
      exampleApp_xmlElem->LinkEndChild(commsReconnect_xmlElem);

      XMLElement* commandPollInterval_xmlElem = xmlDoc->NewElement(XMLTAG_COMMAND_POLL_INTERVAL.c_str());
      buffer.clear();
      buffer.str("");
      buffer << (int)m_commandPollInterval;
      XMLText* commandPollInterval_val = xmlDoc->NewText(std::string(buffer.str()).c_str());
      commandPollInterval_xmlElem->LinkEndChild(commandPollInterval_val);
      exampleApp_xmlElem->LinkEndChild(commandPollInterval_xmlElem);

      XMLElement* commQueueSecondsInterval_xmlElem = xmlDoc->NewElement(XMLTAG_COMMS_QUEUE_SECONDS_INTERVAL.c_str());
      buffer.clear();
      buffer.str("");
      buffer << (int)m_commQueueSecondsInterval;
      XMLText* commQueueSecondsInterval_val = xmlDoc->NewText(std::string(buffer.str()).c_str());
      commQueueSecondsInterval_xmlElem->LinkEndChild(commQueueSecondsInterval_val);
      exampleApp_xmlElem->LinkEndChild(commQueueSecondsInterval_xmlElem);

      XMLElement* deviceSerialNumber_xmlElem = xmlDoc->NewElement(XMLTAG_DEVICE_SERIAL_NUMBER.c_str());
      XMLText* deviceSerialNumber_val = xmlDoc->NewText(m_deviceSerialNumber.c_str());
      deviceSerialNumber_xmlElem->LinkEndChild(deviceSerialNumber_val);
      exampleApp_xmlElem->LinkEndChild(deviceSerialNumber_xmlElem);

      XMLElement* dns_xmlElem = xmlDoc->NewElement(XMLTAG_DNS.c_str());
      XMLText* dns_val = xmlDoc->NewText(m_DNS.c_str());
      dns_xmlElem->LinkEndChild(dns_val);
      exampleApp_xmlElem->LinkEndChild(dns_xmlElem);

      XMLElement* gateway_xmlElem = xmlDoc->NewElement(XMLTAG_GATEWAY.c_str());
      XMLText* gateway_val = xmlDoc->NewText(m_gateway.c_str());
      gateway_xmlElem->LinkEndChild(gateway_val);
      exampleApp_xmlElem->LinkEndChild(gateway_xmlElem);

      XMLElement* ip_xmlElem = xmlDoc->NewElement(XMLTAG_IP.c_str());
      XMLText* ip_val = xmlDoc->NewText(m_ip.c_str());
      ip_xmlElem->LinkEndChild(ip_val);
      exampleApp_xmlElem->LinkEndChild(ip_xmlElem);

      XMLElement* subnet_xmlElem = xmlDoc->NewElement(XMLTAG_SUBNET.c_str());
      XMLText* subnet_val = xmlDoc->NewText(m_subnet.c_str());
      subnet_xmlElem->LinkEndChild(subnet_val);
      exampleApp_xmlElem->LinkEndChild(subnet_xmlElem);

      XMLElement* logEntryLevel_xmlElem = xmlDoc->NewElement(XMLTAG_LOG_ENTRY_LEVEL.c_str());
      XMLText* logEntryLevel_val = xmlDoc->NewText(LogEntry::LOGLEVEL_NAMES[m_logEntryLevel].getName().c_str());
      logEntryLevel_xmlElem->LinkEndChild(logEntryLevel_val);
      exampleApp_xmlElem->LinkEndChild(logEntryLevel_xmlElem);

      XMLElement* queueFilename_xmlElem = xmlDoc->NewElement(XMLTAG_QUEUE_FILENAME.c_str());
      XMLText* queueFilename_val = xmlDoc->NewText(m_queueFilename.c_str());
      queueFilename_xmlElem->LinkEndChild(queueFilename_val);
      exampleApp_xmlElem->LinkEndChild(queueFilename_xmlElem);

      XMLElement* rtcDevicePath_xmlElem = xmlDoc->NewElement(XMLTAG_RTCDEVICE_PATH.c_str());
      XMLText* rtcDevicePath_val = xmlDoc->NewText(m_rtcDevicePath.c_str());
      rtcDevicePath_xmlElem->LinkEndChild(rtcDevicePath_val);
      exampleApp_xmlElem->LinkEndChild(rtcDevicePath_xmlElem);

      XMLElement* debugPrintMask_xmlElem = xmlDoc->NewElement(XMLTAG_DEBUG_PRINT_MASK.c_str());
      buffer.clear();
      buffer.str("");
      buffer << hex << "0x" << uppercase << (int)m_debugPrintMask;
      XMLText* debugPrintMask_val = xmlDoc->NewText(buffer.str().c_str());
      debugPrintMask_xmlElem->LinkEndChild(debugPrintMask_val);
      exampleApp_xmlElem->LinkEndChild(debugPrintMask_xmlElem);

      XMLPrinter configXmlPrinter;
      exampleApp_xmlElem->Accept(&configXmlPrinter);

      // capture the string representation of the XML SAX that we just created.
      std::string configXMLStr(configXmlPrinter.CStr());

      return configXMLStr;
   }

   std::string NetworkConfigSection::getValueByName(const std::string name)
   {
      std::string value = "";
      if (0 == strncasecmp(XMLATTR_VERSION.c_str(), name.c_str(), XMLATTR_VERSION.length()))
      {
         value = static_cast<ostringstream*>( &(ostringstream() << m_version))->str();
      }
      else if (0 == strncasecmp(XMLTAG_ACCESS_KEY.c_str(), name.c_str(), XMLTAG_ACCESS_KEY.length()))
      {
         value = m_accessKey.ToString();
      }
      else if (0 == strncasecmp(XMLTAG_COMMS_RECONNECT_INTERVAL.c_str(), name.c_str(), XMLTAG_COMMS_RECONNECT_INTERVAL.length()))
      {
         value = static_cast<ostringstream*>( &(ostringstream() << m_commsReconnectInterval_seconds))->str();
      }
      else if (0 == strncasecmp(XMLTAG_COMMAND_POLL_INTERVAL.c_str(), name.c_str(), XMLTAG_COMMAND_POLL_INTERVAL.length()))
      {
         value = static_cast<ostringstream*>( &(ostringstream() << m_commandPollInterval))->str();
      }
      else if (0 == strncasecmp(XMLTAG_COMMS_QUEUE_SECONDS_INTERVAL.c_str(), name.c_str(), XMLTAG_COMMS_QUEUE_SECONDS_INTERVAL.length()))
      {
         value = static_cast<ostringstream*>( &(ostringstream() << m_commQueueSecondsInterval))->str();
      }
      else if (0 == strncasecmp(XMLTAG_DEVICE_SERIAL_NUMBER.c_str(), name.c_str(), XMLTAG_DEVICE_SERIAL_NUMBER.length()))
      {
         value = m_deviceSerialNumber.c_str();
      }
      else if (0 == strncasecmp(XMLTAG_DNS.c_str(), name.c_str(), XMLTAG_DNS.length()))
      {
         // TODO: verify that this copies the contents of the string and not
         //       just the location of the string pointer.
         value = m_DNS.c_str();
      }
      else if (0 == strncasecmp(XMLTAG_GATEWAY.c_str(), name.c_str(), XMLTAG_GATEWAY.length()))
      {
         value = m_gateway.c_str();
      }
      else if (0 == strncasecmp(XMLTAG_IP.c_str(), name.c_str(), XMLTAG_IP.length()))
      {
         value = m_ip.c_str();
      }
      else if (0 == strncasecmp(XMLTAG_SUBNET.c_str(), name.c_str(), XMLTAG_SUBNET.length()))
      {
         value = m_subnet.c_str();
      }
      else if (0 == strncasecmp(XMLTAG_LOG_ENTRY_LEVEL.c_str(), name.c_str(), XMLTAG_LOG_ENTRY_LEVEL.length()))
      {
         value = LogEntry::LOGLEVEL_NAMES[m_logEntryLevel].getName();
      }
      else if (0 == strncasecmp(XMLTAG_QUEUE_FILENAME.c_str(), name.c_str(), XMLTAG_QUEUE_FILENAME.length()))
      {
         value = m_queueFilename;
      }
      else if (0 == strncasecmp(XMLTAG_RTCDEVICE_PATH.c_str(), name.c_str(), XMLTAG_RTCDEVICE_PATH.length()))
      {
         value = m_rtcDevicePath;
      }
      else if (0 == strncasecmp(XMLTAG_DEBUG_PRINT_MASK.c_str(), name.c_str(), XMLTAG_DEBUG_PRINT_MASK.length()))
      {
         value = static_cast<ostringstream*>( &(ostringstream() << m_debugPrintMask))->str();
      }

      return value;
   }

   bool NetworkConfigSection::setValueByName(const std::string name, const std::string value)
   {
      // TODO: change all comparisons to ignore case.  ToUpper, ToLower, or case
      //       insensitive compare.

      // TODO: range check values

      int tempVal = 0;
      bool success = true;

      if (0 == strncasecmp(XMLATTR_VERSION.c_str(), name.c_str(), XMLATTR_VERSION.length()))
      {
         stringstream(value) >> tempVal;
         m_version = tempVal;
      }
      else if (0 == strncasecmp(XMLTAG_ACCESS_KEY.c_str(), name.c_str(), XMLTAG_ACCESS_KEY.length()))
      {
         m_accessKey.Set(value);
      }
      else if (0 == strncasecmp(XMLTAG_COMMS_RECONNECT_INTERVAL.c_str(), name.c_str(), XMLTAG_COMMS_RECONNECT_INTERVAL.length()))
      {
         stringstream(value) >> tempVal;
         m_commsReconnectInterval_seconds = tempVal;
      }
      else if (0 == strncasecmp(XMLTAG_COMMAND_POLL_INTERVAL.c_str(), name.c_str(), XMLTAG_COMMAND_POLL_INTERVAL.length()))
      {
         stringstream(value) >> tempVal;
         m_commandPollInterval = tempVal;
      }
      else if (0 == strncasecmp(XMLTAG_COMMS_QUEUE_SECONDS_INTERVAL.c_str(), name.c_str(), XMLTAG_COMMS_QUEUE_SECONDS_INTERVAL.length()))
      {
         stringstream(value) >> tempVal;
         m_commQueueSecondsInterval = tempVal;
      }
      else if (0 == strncasecmp(XMLTAG_DNS.c_str(), name.c_str(), XMLTAG_DNS.length()))
      {
         // TODO: verify that this copies the contents of the string and not
         //       just the location of the string pointer.
         m_DNS = value.c_str();
      }
      else if (0 == strncasecmp(XMLTAG_GATEWAY.c_str(), name.c_str(), XMLTAG_GATEWAY.length()))
      {
         m_gateway = value.c_str();
      }
      else if (0 == strncasecmp(XMLTAG_IP.c_str(), name.c_str(), XMLTAG_IP.length()))
      {
         m_ip = value.c_str();
      }
      else if (0 == strncasecmp(XMLTAG_SUBNET.c_str(), name.c_str(), XMLTAG_SUBNET.length()))
      {
         m_subnet = value;
      }
      else if (0 == strncasecmp(XMLTAG_LOG_ENTRY_LEVEL.c_str(), name.c_str(), XMLTAG_LOG_ENTRY_LEVEL.length()))
      {
         for (uint8_t i = 0; i < MAX_LOG_LEVELS; i++)
         {
            if (0 == strncasecmp(value.c_str(), LogEntry::LOGLEVEL_NAMES[i].getName().c_str(), value.length()))
            {
               m_logEntryLevel = (eLogLevel)LogEntry::LOGLEVEL_NAMES[i].getType();
               break;
            }
         }
      }
      else if (0 == strncasecmp(XMLTAG_DEVICE_SERIAL_NUMBER.c_str(), name.c_str(), XMLTAG_DEVICE_SERIAL_NUMBER.length()))
      {
         m_deviceSerialNumber = value;
      }
      else if (0 == strncasecmp(XMLTAG_QUEUE_FILENAME.c_str(), name.c_str(), XMLTAG_QUEUE_FILENAME.length()))
      {
         m_queueFilename = value;
      }
      else if (0 == strncasecmp(XMLTAG_RTCDEVICE_PATH.c_str(), name.c_str(), XMLTAG_RTCDEVICE_PATH.length()))
      {
         m_rtcDevicePath = value;
      }
      else if (0 == strncasecmp(XMLTAG_DEBUG_PRINT_MASK.c_str(), name.c_str(), XMLTAG_DEBUG_PRINT_MASK.length()))
      {
         stringstream(value) >> tempVal;
         m_debugPrintMask = tempVal;
      }
      else
      {
         getLogDriver()->Log("Nothing set!", LoggingError);
      }

      return success;
   }

   bool NetworkConfigSection::readConfigFile(string configFilePath __attribute((unused)))
   {
      getLogDriver()->Log("NetworkConfigSection::readConfigFile Unimplemented.", LoggingDebug);

      return false;
   }

   bool NetworkConfigSection::writeConfigFile(string configFilePath __attribute((unused)))
   {
      getLogDriver()->Log("NetworkConfigSection::readConfigFile Unimplemented.", LoggingDebug);

      return false;
   }
}
