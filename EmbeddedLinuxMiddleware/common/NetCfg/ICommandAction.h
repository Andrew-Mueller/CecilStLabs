#ifndef ICOMMANDACTION_H_
#define ICOMMANDACTION_H_

namespace CecilStLabs
{
   /**
    * Interface for a command action to take.
    */
   class ICommandAction
   {
      public:
         virtual bool execute(const string& command) = 0;

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
