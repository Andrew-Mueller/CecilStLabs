#include "../basicTypes.h"
#include "../util/EnumByName.h"
#include "../DateTime/DateTime.h"
#include "../util/BasicUtil.h"

#include "../LinkedList/SingleLink.h"
#include "../LinkedList/SingleLinkedList.h"

#include "Transition.h"
#include "ITransitionHandler.h"
#include "IState.h"
#include "StateMachine.h"

namespace CecilStLabs
{
   StateMachine::StateMachine() :
      m_currentState(NULL),
      m_states()
   {
      // intentionally left blank.
   }

   StateMachine::~StateMachine()
   {
      // intentionally left blank.
   }

   bool StateMachine::add(IState* state)
   {
      bool stateAdded = false;

      if (NULL != state)
      {
         // the fist state added becomes the current state.
         if (NULL == m_currentState)
         {
            m_currentState = state;

            // call enter so first state's enter is called
            m_currentState->enter();
         }

         // also add the specified state to the list of states.
         stateAdded = m_states.insert((SingleLink*)state);
      }

      return stateAdded;
   }

   void StateMachine::signal(uint32_t signal)
   {
      if (m_currentState != NULL)
      {
         m_currentState->signal(signal);
      }
   }

   void StateMachine::execute()
   {
      if (m_currentState != NULL)
      {
         m_currentState->execute();
      }
   }

   void StateMachine::handle(IState* newState)
   {
      // verify the specified state is in fact in the state machine.

      if ((NULL != newState) &&
          m_states.inList(newState))
      {
         // NOTE: don't call the exit and enter methods if we are already in the
         //       target state.
         if (newState != m_currentState)
         {
            if (NULL != m_currentState)
            {
               m_currentState->exit();
            }

            m_currentState = newState;

            m_currentState->enter();
         }
      }
   }
}
