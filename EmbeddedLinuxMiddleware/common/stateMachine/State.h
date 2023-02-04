#ifndef STATE_H_
#define STATE_H_

namespace CecilStLabs
{
   /**
    * Abstract base state class to define common state functionality.
    */
   class State : public IState
   {
      public:

         /**
          * Logic that is run once, when entering the state.
          * No common enter logic, so leave the implementation up to the child.
          */
         virtual void enter() = 0;

         /**
          * Logic that is run each iteration through the state machine, while
          * currently in the state.
          * No common execute logic, so leave the implementation up to the child.
          */
         virtual void execute() = 0;

         /**
          * Logic that is run once, when exiting the state.
          * No common exit logic, so leave the implementation up to the child.
          */
         virtual void exit() = 0;

         /**
          * Adds the specified transition to the state.
          *
          * @param transition Defines the between this state and the next.
          */
         void addTransition(Transition* transition);

         /**
          * Sets the transition handler to be used when this state receives a
          * signal for one of its transitions.
          *
          * @param transitionHandler The object implementing the transition handler interface
          *        called when we need to transition to a new state.
          */
         void setTransitionHandler(ITransitionHandler* transitionHandler);

         /**
          * Send the specified signal to the state.
          *
          * @param signalValue signal sent to the state.
          *        This is expected to be a member of the signals enumeration.
          */
         void signal(uint32_t signalValue);

      protected:

         // NOTE: define the constructor, copy constructor, destructor and
         //       assignment operator as protected to ensure an instance of the
         //       abstract class cannot be created.

         State() : m_transitionList(), m_transitionHandler(NULL), m_inState(false) {};
         State(State* copy __attribute__((unused))) : m_transitionList(), m_transitionHandler(NULL), m_inState(false) { };
         virtual ~State() {};

         State* operator=(State* copy __attribute__((unused))) { return 0; };

         /**
          * List containing the transitions out of this state.
          */
         SingleLinkedList m_transitionList;

         /**
          * The handler to call when this state receives a signal for one of its
          * transitions.
          */
         ITransitionHandler* m_transitionHandler;

        /**
          * flag to indicate it really is in this state
          */
         bool m_inState;

   };

}
#endif
