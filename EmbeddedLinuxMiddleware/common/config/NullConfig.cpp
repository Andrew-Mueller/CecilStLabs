#include <string>
using namespace std;

#include "../tinyxml/tinyxml2.h"
using namespace tinyxml2;

#include "IConfig.h"
#include "NullConfig.h"

namespace CecilStLabs
{

   NullConfig::NullConfig()
   {
      // intentionally left blank.
   }

   NullConfig::~NullConfig()
   {
      // intentionally left blank.
   }

   bool NullConfig::readSection(std::string configStr __attribute((unused)))
   {
      return false;
   }

   std::string NullConfig::writeSection(XMLNode &xmlNode __attribute((unused)))
   {
      return std::string("");
   }

   std::string NullConfig::getValueByName(const std::string name __attribute((unused)))
   {
      return std::string("");
   }

   bool NullConfig::setValueByName(const std::string name __attribute((unused)), const std::string value __attribute((unused)))
   {
      return true;
   }

   bool NullConfig::readConfigFile(string configFilePath __attribute((unused)))
   {
      return false;
   }

   bool NullConfig::writeConfigFile(string configFilePath __attribute((unused)))
   {
      return false;

   }

}
