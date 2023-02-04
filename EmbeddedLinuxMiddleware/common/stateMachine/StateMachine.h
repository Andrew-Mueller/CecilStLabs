#ifndef STATEMACHINE_H_
#define STATEMACHINE_H_

namespace CecilStLabs
{
   /**
    * The general purpose state machine contains states and their transitions to move
    * between those states.  A transistion will move to a new state when a
    * specified transitions signal is sent.
    */
   class StateMachine : public ITransitionHandler
   {
      public:
         StateMachine();
         virtual ~StateMachine();

         /**
          * Add the specified state to this state machine.
          *
          * @param state State to add to the state machine.
          *
          * @return True if the specified state was added, False if the state
          *         was not added.
          */
         bool add(IState* state);

         /**
          * Sends a signal to the current state.
          *
          * @param signal The signal to send to the current state.
          *        Expected to be a member of the application specific signals
          *        enumeration.
          */
         void signal(uint32_t signal);

         /**
          * Execute the "do" work of the current state.
          */
         void execute();

         /**
          * Implementation of the ITransitionHandler.
          * Performs the transition from one state to another by calling the
          * current state's exit method, sets the current state as the newState
          * and performs the newState's enter method.
          *
          * @param newState The state we are transitioning to.
          *              NOTE: newState is expected to have been previously added
          *                    to this state machine.
          */
         void handle(IState* newState);

      protected:

         /**
          * The current state of the state machine.
          */
         IState* m_currentState;

         /**
          * List of all of the possible states that make up this state machine.
          */
         SingleLinkedList m_states;
   };
}
#endif
