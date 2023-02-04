#ifndef XMLFILETABLE_H_
#define XMLFILETABLE_H_

namespace CecilStLabs
{
   /**
    * Class to read XML file that has a table type of set of values
    */
   class XMLFileTable : public ILoggable
   {
      public:
         /**
          * constructor for class
          *
          * @param file file name of XML file
          */
         XMLFileTable(const string& file, const string& rootSection, const string& elements);

         /**
          * destructor
          */
         virtual ~XMLFileTable();

         /**
          * Read the XML file with values into memory
          */
         void readFile();

         /**
          * Write the XML from memory to the file specified on creation
          */
         void writeFile();

         uint8_t getVersion()
         {
            return m_version;
         }

      protected:

         /**
          * Read the  section specified by the section name.
          *
          * @param configStr String containing the section of XML to read.
          * @return True if all of the data was successfully read, false if an error occurred.
          */
         bool readSection(std::string section);

         /**
          * Writes the configuration section and returns the XML string
          * representation from the config.
          *
          * @param The parent node that this configuration is being added to.
          *
          * @return String representation of the XML config section.
          */
         std::string writeSection(XMLNode& xmlParentNode);

         /**
          * read the object specific parameters
          */
         virtual bool readParameters(XMLElement* elem) = 0;

          /**
          * Writes the list of paraeters and returns the XML string
          * representation from the config.
          *
          * @param The parent node that this list is being added to.
          *
          * @return true if success
          */
         virtual bool writeParameters(XMLNode &xmlParentNode);

         /**
          * File name passed into object
          */
         string m_fileName;

         /**
          * The name of the element containing all other elements in the table.
          */
         string m_rootSection;

         /**
          * The name of individual element
          */
         string m_elementName;

         /**
          * The version number of the configuration file format.
          * If a configuration value is added, removed, or changed the version
          * number must be incremented so the proper logic can be used to
          * manipulate the configuration.
          */
         static const std::string XMLATTR_VERSION;
         uint8_t m_version;

         /**
          * This is the calculated 16bit CRC calculated for the configuration
          * sections contained within this config.
          *
          * If any value is changed within the section, this is recalculated.
          */
         static const std::string XMLATTR_CRC16;
         uint16_t m_crc16;

   };
}
#endif
