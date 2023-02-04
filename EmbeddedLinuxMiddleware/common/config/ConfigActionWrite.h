#ifndef CONFIGACTIONWRITE_H_
#define CONFIGACTIONWRITE_H_

namespace CecilStLabs
{
   class ConfigActionWrite : public IConfigAction
   {
      public:
         ConfigActionWrite();
         virtual ~ConfigActionWrite();

         bool execute(const XMLElement* root,
                            std::string elementName,
                            std::stringstream& buffer);
      protected:
         ConfigActionWrite(IConfigAction& copy __attribute__((unused))) {};
         ConfigActionWrite* operator=(ConfigActionWrite* rhs) { return rhs; };

   };
}

#endif
