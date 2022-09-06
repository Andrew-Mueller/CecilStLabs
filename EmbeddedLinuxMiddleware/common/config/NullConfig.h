#ifndef NULLCONFIG_H_
#define NULLCONFIG_H_

namespace CecilStLabs
{
   /**
    * Provide an empty implementation of the IConfig interface to use if needed
    * in place of a concrete implementation of IConfig.
    */
   class NullConfig : public IConfig
   {
      public:
         NullConfig();
         virtual ~NullConfig();

         // implement the IConfig interface by implementing the last three functions.
         bool readSection(std::string configStr);

         std::string writeSection(XMLNode &xmlNode);

         std::string getValueByName(const std::string name);

         bool setValueByName(const std::string name, const std::string value);

         bool readConfigFile(std::string configFilePath);

         bool writeConfigFile(std::string configFilePath);
   };
}
#endif
