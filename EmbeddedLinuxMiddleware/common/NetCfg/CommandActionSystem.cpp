#include <string>
#include <iostream>
#include <sstream>
#include <cstdlib>
using namespace std;

#include "ICommandAction.h"
#include "CommandActionSystem.h"

namespace CecilStLabs
{
   CommandActionSystem::CommandActionSystem()
   {
      // intentionally left blank
   }

   CommandActionSystem::~CommandActionSystem()
   {
      // intentionally left blank
   }

   bool CommandActionSystem::execute(const string& commmand)
   {
      return (system( commmand.c_str() >= 0)
   }
}
