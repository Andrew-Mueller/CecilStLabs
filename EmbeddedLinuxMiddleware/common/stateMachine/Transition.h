#ifndef TRANSITION_H_
#define TRANSITION_H_

namespace CecilStLabs
{
   /**
    * Forward declaration of the IState interface which is used by the allowing
    * Transition class, but IState also needs to know about Transitions.
    *
    * This forward declaration is to avoid circular references.
    */
   class IState;

   /**
    * A State Machine Transition defines how one state moves to another.  A
    * Transition is defined by the signal, and the state to switch too when that
    * signal is received.
    *
    * NOTE: A Transition inherits from the SingleLink class so it can be put
    *       into a Single Linked List.  Transitions are created and then added
    *       to state's list of transition.
    */
   class Transition : public SingleLink
   {
      public:

         /**
          * Default constructor sets up the transition with an empty signal
          * and empty target state.
          */
         Transition();

         /**
          * Create the transition with the specified signal to the specified state.
          *
          * @param signal The enumerated signal that will cause the the
          *               transition to the target state.
          * @param targetState The state to transition to when the signal is
          *                    received.
          */
         Transition(const uint32_t signal, IState* targetState);
         virtual ~Transition();

         // getter mutators of the class members.
         inline uint32_t getSignal() const { return m_signal; };
         inline IState* getTargetState() const { return m_targetState; };

      protected:

         /**
          * The signal that will allow the state to change to the target state.
          * NOTE: this is actually populated with an application specific
          *       enumeration.
          */
         uint32_t m_signal;

         /**
          * The state the transition results in changing to.
          */
         IState* m_targetState;

   };
}

#endif
