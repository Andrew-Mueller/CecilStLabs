#ifndef CONFIGACTIONREAD_H_
#define CONFIGACTIONREAD_H_

namespace CecilStLabs
{
   /**
    * Config Action used for reading the configuration.
    */
   class ConfigActionRead : public IConfigAction
   {
      public:

         /**
          * Create the configuration action read implementation of the
          * IConfigAction interface.
          */
         ConfigActionRead();

         /**
          * Release the resources of this specific implementation of the
          * IConfigAction interface realization.
          */
         virtual ~ConfigActionRead();

         /**
          * Read the value of the specified XML element with the XML root specified into
          * the buffer specified.
          *
          * @param root XML root to start looking for the specified elementName into.
          * @param elementName Name of the XML Element to read the value into.
          * @param buffer string stream containing the value of the element read.
          * @return True if the configuration value was read successfully, false
          *         if it was not (the contents of buffer is undefined).
          */
         bool execute(const XMLElement* root,
                            std::string elementName,
                            std::stringstream& buffer);

      protected:

         // define the copy constructor and assignment operator as protected
         // to ensure public versions of these are not created by default by
         // the c++ compiler.
         ConfigActionRead(IConfigAction& copy __attribute__((unused))) {};
         ConfigActionRead* operator=(ConfigActionRead* rhs) { return rhs; };
   };
}

#endif
