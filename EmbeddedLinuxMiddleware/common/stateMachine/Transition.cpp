#include "../basicTypes.h"
#include "../util/EnumByName.h"
#include "../DateTime/DateTime.h"
#include "../util/BasicUtil.h"

#include "../LinkedList/SingleLink.h"

#include "IState.h"
#include "Transition.h"

namespace CecilStLabs
{
   Transition::Transition()
      : m_signal(0),
        m_targetState(NULL)
   {
      // intentionally left blank.
   }

   Transition::Transition(const uint32_t signal, IState* targetState)
      : m_signal(signal),
        m_targetState(targetState)

   {
      // intentionally left blank.
   }

   Transition::~Transition()
   {
      // intentionally left blank.
   }
}
