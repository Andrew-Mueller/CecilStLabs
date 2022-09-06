/**
 * LocalShellStates.cpp
 *
 * This is the shell application state fucntionality
 *
 * The states are responsible for processing actual commands
 */

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>


#include <cstdio>
#include <cstring>
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>

using namespace std;

#include "../../common/tinyxml/tinyxml2.h"
using namespace tinyxml2;

#include "../../common/basicTypes.h"
#include "../../common/util/EnumByName.h"
#include "../../common/DateTime/DateTime.h"
#include "../../common/util/BasicUtil.h"
#include "../../common/guid.h"

#include "../../common/logging/LogEntry.h"
#include "../../common/logging/ILogDriver.h"
#include "../../common/logging/emptyLogDriver.h"
#include "../../common/logging/ILoggable.h"

#include "../../common/config/IConfig.h"
#include "../../common/config/IConfigAction.h"
#include "../../common/config/ConfigActionRead.h"
#include "../../common/config/ConfigActionWrite.h"
#include "../../common/config/NetworkConfigSection.h"


#include "../../common/NetCfg/ICommandAction.h"
#include "../../common/NetCfg/CommandActionSystem.h"

#include "LocalShellBaseHandler.h"
#include "LocalShellNormalHandler.h"

namespace CecilStLabs
{

   LocalShellNormalHandler::LocalShellNormalHandler( IConfig& config ) :
       LocalShellBaseHandler(config)
   {
      m_type = HANDLER_NORMAL;
      m_prompt = "$ ";
   }

   LocalShellNormalHandler::~LocalShellNormalHandler()
   {
      // intentionally left blank
   }


   eShellHandlers LocalShellNormalHandler::processCommand( const string& command, const char delim, ICommandAction* action )
   {
      eShellHandlers retHandler = HANDLER_INVALID;
      vector<string> elems;

      split( command, delim, elems );

      if( elems.size() == 1 && elems[0] == "net" )
      {
         retHandler = HANDLER_NET;
      }
      // TODO - there will be more commands added here

      if( HANDLER_INVALID == retHandler )
      {
         retHandler = LocalShellBaseHandler::processCommand( command, delim, action );
      }

      return retHandler;
   }

   // If there is a derived class, make sure this gets called first from overwritten function
   void LocalShellNormalHandler::cmdHelp()
   {
      LocalShellBaseHandler::cmdHelp();

      cout << "  net       - enter network config submenu" << endl;

   }
}
