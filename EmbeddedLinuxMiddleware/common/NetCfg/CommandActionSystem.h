#ifndef COMMANDACTIONSYSTEM_H_
#define COMMANDACTIONSYSTEM_H_

namespace CecilStLabs
{
   /**
    * Concrete implementation of the command action interface specifically for system calls.
    */
   class CommandActionSystem : public ICommandAction
   {
      public:
         CommandActionSystem();
         virtual ~CommandActionSystem();

         // implementation of the ICommandAction interface.
         bool execute(const std::string& command);

      protected:
         CommandActionSystem(ICommandAction& copy __attribute__((unused))) {};
         CommandActionSystem* operator=(CommandActionSystem* rhs) { return rhs; };

   };
}

#endif
