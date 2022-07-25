#ifndef ISTATE_H_
#define ISTATE_H_

namespace CecilStLabs
{
   // forward declaration of the transition class to avoid circular includes.
   class Transition;

   /**
    * Abstract base class for a concrete state.
    * Used for building up the deterministic rules of the system.
    */
   class IState : public SingleLink
   {
      public:

         /**
          * Logic to perform when entering the state.
          */
         virtual void enter() = 0;

         /**
          * Logic to perform while in the state.
          */
         virtual void execute() = 0;

         /**
          * Logic to perform when exiting the state.
          */
         virtual void exit() = 0;

         /**
          * Adds the specified transition to the state.
          *
          * @param transition Defines the between this state and the next.
          */
         virtual void addTransition(Transition* transition) = 0;

         /**
          * Send the specified signal to the state.
          *
          * @param signal Signal sent to the state.
          *               This is expected to be a member of the signals enumeration.
          */
         virtual void signal(uint32_t signal) = 0;

      protected:

         /**
          * define the constructor and destructor as non-public to ensure an
          * instance of the interface can not be created.
          */
         IState() {};
         IState(IState* copy __attribute__((unused))) {};
         virtual ~IState() {};
         IState* operator=(IState* copy __attribute__((unused))) {return NULL;};
   };
}

#endif
