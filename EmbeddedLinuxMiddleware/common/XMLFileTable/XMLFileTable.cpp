#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include <sstream>
#include <iomanip>

using namespace std;

#include "../tinyxml/tinyxml2.h"
using namespace tinyxml2;

#include "../basicTypes.h"

#include "../util/EnumByName.h"
#include "../DateTime/DateTime.h"

#include "../logging/LogEntry.h"
#include "../logging/ILogDriver.h"
#include "../logging/emptyLogDriver.h"
#include "../logging/ILoggable.h"

#include "../util/BasicUtil.h"

#include "XMLFileTable.h"

namespace CecilStLabs
{
   const std::string XMLFileTable::XMLATTR_VERSION = "version";
   const std::string XMLFileTable::XMLATTR_CRC16 = "CRC16";

   XMLFileTable::XMLFileTable(const string& file, const string& rootSection, const string& elements)
         : m_fileName(file),
           m_rootSection(rootSection),
           m_elementName(elements),
           m_version(0),
           m_crc16(0)
   {
      // intentionally left blank
   }

   XMLFileTable::~XMLFileTable()
   {
      // intentionally left blank
   }

   void XMLFileTable::readFile()
   {
      bool success = false;

      if (BasicUtil::FileExists(m_fileName))
      {
         // read all of the contents of the temp xml file into the string.
         std::ifstream tempFileStream(m_fileName.c_str());
         std::string fileContent;
         fileContent.assign((std::istreambuf_iterator<char>(tempFileStream)),
                            (std::istreambuf_iterator<char>()));

         success = this->readSection(fileContent);

         if (false == success)
         {
            getLogDriver()->Log(string("read section failed"), LoggingError);
         }
      }
      else
      {
         getLogDriver()->Log(string("table file did not exist"), LoggingError);
      }

   }

   void XMLFileTable::writeFile()
   {
      // create the object that will manage the entire document.
      XMLDocument xmlDoc;

      // the xml decleration
      XMLDeclaration* decl = xmlDoc.NewDeclaration("xml version=\"1.0\" encoding=\"UTF-8\"");
      xmlDoc.LinkEndChild(decl);

      XMLNode* config_xmlELem = xmlDoc.NewElement(m_rootSection.c_str());
      xmlDoc.LinkEndChild(config_xmlELem);

      std::string resultXML = this->writeSection((XMLNode&)(*config_xmlELem));

      // simply check that 'something' was written
      if (!resultXML.empty())
      {
         // save the newly constructed configuration to a file.
         xmlDoc.SaveFile(m_fileName.c_str());
      }
   }

   bool XMLFileTable::writeParameters(XMLNode &xmlParentNode)
   {
      return true;
   }

   bool XMLFileTable::readSection(string configStr)
   {
      bool success = false;
      XMLDocument xmlDoc;
      XMLError parseStatus = xmlDoc.Parse(configStr.c_str());

      if (XML_NO_ERROR == parseStatus)
      {
         XMLElement* root = xmlDoc.RootElement();

         if (NULL != root)
         {
            istringstream buffer;

            unsigned int tempReadUInt = 0;

            // read the version number of the configuration
            const char* readAttribute = root->Attribute(XMLATTR_VERSION.c_str());
            if (NULL != readAttribute)
            {
               string versionStr(readAttribute);

               if (!versionStr.empty())
               {
                  buffer.str(versionStr);
                  buffer >> tempReadUInt;
                  m_version = tempReadUInt;
               }
            }

            // read the CRC for the contained sections.
            readAttribute = root->Attribute(XMLATTR_CRC16.c_str());
            if (NULL != readAttribute)
            {
               string crcStr(readAttribute);

               if (!crcStr.empty())
               {
                  stringstream streamBuff;
                  streamBuff << std::hex << crcStr.c_str();
                  streamBuff >> tempReadUInt;
                  m_crc16 = tempReadUInt;

                  // TODO: this crc value needs to be compared against the
                  //       calculated CRC of all of the values in the configuration.
               }
            }

            XMLElement* temp = root->FirstChildElement(m_elementName.c_str());
            while (NULL != temp)
            {
               success = readParameters(temp);

               temp = temp->NextSiblingElement();
            }
         }
      }

      return success;
   }

   std::string XMLFileTable::writeSection(XMLNode& xmlParentNode)
   {
      std::string xmlConfig("");

      XMLElement* config_xmlElem = xmlParentNode.ToElement();
      BasicUtil::Assert((NULL != config_xmlElem), __FILE__, __LINE__);
      stringstream buffer;
      buffer.clear();
      buffer.str("");
      buffer << std::setw(3) << std::setfill('0') << (int)m_version;
      config_xmlElem->SetAttribute(XMLATTR_VERSION.c_str(), buffer.str().c_str());

      XMLNode* configNode = (XMLNode*)config_xmlElem;

      writeParameters( *configNode );

      // TODO: calculate the entire config's CRC.
      buffer.clear();
      buffer.str("");
      buffer << std::setw(4) << std::setfill('0') << "0x" << (int)m_crc16;
      config_xmlElem->SetAttribute(XMLATTR_CRC16.c_str(), buffer.str().c_str());

      // capture the string representation of the XML SAX that we just created.
      XMLPrinter configXmlPrinter;
      config_xmlElem->Accept(&configXmlPrinter);
      xmlConfig = configXmlPrinter.CStr();

      return xmlConfig;
   }
}
