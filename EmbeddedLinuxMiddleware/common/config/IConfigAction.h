#ifndef ICONFIGACTION_H_
#define ICONFIGACTION_H_

namespace CecilStLabs
{
   /**
    * Interface for an action to take on a configuration value; for example,
    * reading or writing the value of a configuration.
    */
   class IConfigAction
   {
      public:

         /**
          * Release any resources used by the implementation of the Config Action.
          */
         virtual ~IConfigAction()
         {
            // intentionally left blank.
            // Implementation intended for this interface's realization.
         };

         /**
          * The action to take on the configuration.
          * @param root XML Root to look for elementName as a child of.
          * @param elementName The element to perform the action on.
          * @param buffer contents for the configuration.  For example, new
          *        config value to write, or filled in with the current config
          *        value that was read.
          * @return True if the action succeeded.  False if the action failed.
          *         If false, the contents of the buffer parameter is undefined.
          */
         virtual bool execute(const XMLElement* root,
                                    std::string elementName,
                                    std::stringstream& buffer) = 0;

      protected:
         // constructor, copy, and assignment operator defined
         // protected to ensure an instance of the abstract interface cannot be
         // created.

         IConfigAction() {};
         IConfigAction(IConfigAction& copy __attribute__((unused))) {};
         IConfigAction& operator=(IConfigAction& rhs) { return rhs; };
   };
}

#endif
