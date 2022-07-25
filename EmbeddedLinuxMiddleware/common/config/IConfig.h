#ifndef ICONFIG_H_
#define ICONFIG_H_

namespace CecilStLabs
{
   /**
    * Interface to the configuration classes that will specifically handle their own
    * configuration values.
    */
   class IConfig
   {
      public:

         /**
          * Read the configuration section specified by the section name.
          *
          * @param configStr String containing the section of XML to read.
          * @return True if all of the data was successfully read, false if an error occurred.
          */
         virtual bool readSection(string configStr) = 0;

         /**
          * Writes the configuration section and returns the XML string
          * representation from the config.
          *
          * @return String representation of the XML config section.
          */
         virtual std::string writeSection(XMLNode &xmlNode) = 0;

         /**
          * Retrieves the string-a-fied version representing the value of the
          * configuration setting specified by the name.
          *
          * @param name Name of the configuration value to retrieve.
          *
          * @return String-a-fied version of the Value
          */
         virtual std::string getValueByName(const std::string name) = 0;

         /**
          * Set the value of the configuration setting specified by the name.
          *
          * @param name Name of the configuration value to set.
          *
          * @return true if value was set
          */
         virtual bool setValueByName(const std::string name, const std::string value) = 0;

         /**
          * Reads the Configuration from the specified filename path.
          *
          * @param filename The path to the config file to load settings from.
          *
          * @returns True if the config file was read successfully,
          *          False if an error occurred while reading the configuration file.
          */
         virtual bool readConfigFile(string configFilePath) = 0;

         /**
          * Writes the Configuration to the specified filename path.
          *
          * @param configFilePath The path to the config file to save settings to.
          *
          * @return True if the config file was written successfully,
          *         False if an error occured while writing the configuration file.
          */
         virtual bool writeConfigFile(string configFilePath) = 0;

         /**
          * setter and getter for file path
          */
         string getConfigFilePath() { return m_configFilePath; }
         void setConfigFilePath( const string& path) { m_configFilePath = path; }

      protected:

         /**
          * NOTE: the constructor is protected to ensure that this abstract
          *       configuration object cannot be created.
          */
         IConfig() {};

         /**
          * NOTE: the destructor is protected to ensure that it cannot be called
          *       (since it cannot be created).
          */
         virtual ~IConfig() {};

         /**
          * NOTE: the copy constructor is protected to ensure that this abstract
          *       configuration object cannot be created from an already
          *       existing configuration.
          */
         IConfig(IConfig& copy) { (void)copy;};

         /**
          * path to where config file is stored
          */
         string m_configFilePath;
   };
}

#endif
