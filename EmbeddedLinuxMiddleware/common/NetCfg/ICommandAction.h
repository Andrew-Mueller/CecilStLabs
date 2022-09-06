#ifndef ICOMMANDACTION_H_
#define ICOMMANDACTION_H_

namespace CecilStLabs
{
   /**
    * Interface for a command action to take.
    * An Action is some functionality to execute when a command is parsed.
    */
   class ICommandAction
   {
      public:
         virtual bool execute(const std::string& command) = 0;

         virtual ~ICommandAction() {};

      protected:
         // constructor, copy, destructor, and assignment operator defined
         // protected to ensure an instance of this object cannot be created.

         ICommandAction() {};
         ICommandAction(ICommandAction& copy __attribute__((unused))) {};
         ICommandAction& operator=(ICommandAction& rhs) { return rhs; };
   };
}

#endif
