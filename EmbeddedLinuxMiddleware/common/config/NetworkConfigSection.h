#ifndef EXAMPLECONFIGSECTION_H_
#define EXAMPLECONFIGSECTION_H_

namespace CecilStLabs
{
   /**
    * Specific configuration section.  This is the object representation
    * of the configuration settings read from the XMl file.
    *
    * NOTE: this is an example of how to create a configuration section for an application.
    *       As an example, included some common config info such as IP address, DNS, etc.
    */
   class NetworkConfigSection : public IConfig, public ILoggable
   {
      public:

         /**
          * initialize the configuration object with default values.
          */
         NetworkConfigSection();

         /**
          * release any resources used by the configuration section.
          */
         virtual ~NetworkConfigSection();

         /**
          * Read the configuration section from the config XML file.
          *
          * @param configStr string containing the xml section to read.  Expected to start with
          *        <ExampleConfig> root element.
          * @return True if all of the data was successfully read, false if an error occured.
          */
         bool readSection(std::string configStr);

         /**
          * Writes the configuration section and returns the XML string
          * representation from the config.
          *
          * @return String representation of the XML config section.
          */
         std::string writeSection(XMLNode &xmlNode);

         /**
          * Retrieves the string-a-fied version representing the value of the
          * configuration setting specified by the name.
          *
          * @param name Name of the configuration value to retrieve.
          *
          * @return String-a-fied version of the Value
          */
         std::string getValueByName(const std::string name);

         /**
          * Set the value of the configuration setting specified by the name.
          *
          * @param name Name of the configuration value to set.
          *
          * @return true if value was set
          */
         bool setValueByName(const std::string name, const std::string value);

         /**
          * Reads the Configuration from the specified filename path.
          *
          * @param filename The path to the config file to load settings from.
          *
          * @returns True if the config file was read successfully,
          *          False if an error occurred while reading the configuration file.
          */
         bool readConfigFile(const std::string configFilePath);

         /**
          * Writes the Configuration to the specified filename path.
          *
          * @param configFilePath The path to the config file to save settings to.
          *
          * @return True if the config file was written successfully,
          *         False if an error occured while writing the configuration file.
          */
         bool writeConfigFile(const std::string configFilePath);

         /**
          * getters and setters for each of the config settings. All settings have
          * a getter, but if it doesn't make sense to be able to change a setting
          * (firmware version number for instance) some settings lack a setter in these cases.
          */
         uint8_t getVersion() { return m_version; };

         GUID getAccessKey() { return m_accessKey; };
         void setAccessKey(const GUID guid) { m_accessKey = guid; };

         inline uint8_t getCommsReconnectInterval_Seconds() { return m_commsReconnectInterval_seconds; };

         inline uint8_t getCommandPollInterval() { return m_commandPollInterval; };

         inline uint8_t getCommunicationQueueSecondsInterval() { return m_commQueueSecondsInterval; };

         inline std::string getDeviceSerialNumber() const { return m_deviceSerialNumber; };
         inline void setDeviceSerialNumber(const std::string serialNum) { m_deviceSerialNumber = serialNum; };

         inline std::string getDNS() { return m_DNS; };
         inline void setDNS(const std::string dns) { m_DNS = dns; };

         inline std::string getGateway() { return m_gateway; };
         inline void setGateway(const std::string gateway) { m_gateway = gateway; };

         inline std::string getIP() { return m_ip; };
         inline void setIP(const std::string ip) { m_ip = ip; };

         inline std::string getSubnet() { return m_subnet; };
         inline void setSubnet(const std::string subnet ) { m_subnet = subnet; };

         inline eLogLevel getLogEntryLevel() { return m_logEntryLevel; };

         inline std::string getQueueFilename() { return m_queueFilename; };
         inline std::string getRTCDevicePath() { return m_rtcDevicePath; };

         inline uint8_t getDebugPrintMask() { return m_debugPrintMask; };

         static const std::string XMLTAG_NETWORK_CONFIG;
         static const std::string XMLATTR_VERSION;
         static const std::string XMLTAG_ACCESS_KEY;
         static const std::string XMLTAG_COMMS_RECONNECT_INTERVAL;

         static const std::string XMLTAG_COMMAND_POLL_INTERVAL;
         static const std::string XMLTAG_COMMS_QUEUE_SECONDS_INTERVAL;
         static const std::string XMLTAG_DEVICE_SERIAL_NUMBER;
         static const std::string XMLTAG_DNS;
         static const std::string XMLTAG_GATEWAY;
         static const std::string XMLTAG_IP;
         static const std::string XMLTAG_SUBNET;
         static const std::string XMLTAG_LOG_ENTRY_LEVEL;
         static const std::string XMLTAG_QUEUE_FILENAME;
         static const std::string XMLTAG_RTCDEVICE_PATH;
         static const std::string XMLTAG_DEBUG_PRINT_MASK;

      protected:

         /**
          * The version of this section of the configuration.
          */
         uint8_t m_version;

         /**
          * Device Access Key required for authenticated communication
          * from a Device to the Server.
          */
         GUID m_accessKey;

         /**
          * The number of seconds to reconnect the interval
          */
         static const uint8_t DEFAULT_COMMS_RECONNECT_INTERVAL_SECONDS = 10;
         uint8_t m_commsReconnectInterval_seconds;

         /**
          * The number of minutes to wait between polling for new commands.
          */
         uint8_t m_commandPollInterval;

         /**
          * The number of seconds to wait between processing the communication queue.
          */
         uint8_t m_commQueueSecondsInterval;
         static const uint8_t DEFAULT_COMMUNICATION_QUEUE_SECONDS_INTERVAL = 10;

         /**
          * This device's serial number that uniquely identifies the device
          */
         std::string m_deviceSerialNumber;

         /**
          * The IP address to the domain name lookup server.
          */
         std::string m_DNS;

         /**
          * The IP address to the gateway node to the
          */
         std::string m_gateway;

         /**
          * The IP address of THIS node on the network.
          */
         std::string m_ip;

         /**
          * Mask of the ip address
          */
         std::string m_subnet;

         /**
          * The level set for filtering the Log Entry Events generated by this
          * application.
          */
         eLogLevel m_logEntryLevel;

         /**
          * The filename of the communication file.
          */
         std::string m_queueFilename;
         static const std::string DEFAULT_QUEUE_FILENAME;

         /**
          * The path to the device file for the real time clock.
          */
         std::string m_rtcDevicePath;
         static const std::string DEFAULT_RTC_DEVICE_PATH;

         /**
          * The debug print mask is used to turn on and off the specific debug
          * output tha is logged to the syslog and printed to the local terminal.
          */
         uint8_t m_debugPrintMask;

         static const uint8_t DEFAULT_DEBUG_PRINT_MASK = 0xFF;
   };
}

#endif
