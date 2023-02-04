#include "../basicTypes.h"
#include "../util/EnumByName.h"
#include "../DateTime/DateTime.h"
#include "../util/BasicUtil.h"

#include "../LinkedList/SingleLink.h"
#include "../LinkedList/SingleLinkedList.h"

#include "Transition.h"
#include "ITransitionHandler.h"
#include "IState.h"
#include "State.h"

namespace CecilStLabs
{
   void State::addTransition(Transition* transition)
   {
      m_transitionList.insert(transition, NULL);
   }

   void State::setTransitionHandler(ITransitionHandler* transitionHandler)
   {
      m_transitionHandler = transitionHandler;
   }

   void State::signal(uint32_t signalValue)
   {
      Transition* transition = (Transition*)m_transitionList.first();

      while (NULL != transition)
      {
         if (transition->getSignal() == signalValue)
         {
            // the handler for the transition from this state.

            // Most often this will be set to the StateMachine's transition
            // method and transition from this current state to the transitions
            // target state.

            if (NULL != m_transitionHandler)
            {
               m_transitionHandler->handle(transition->getTargetState());
            }

            transition = NULL;
         }
         else
         {
            transition = (Transition*)m_transitionList.next(transition);
         }
      }
   }
}
