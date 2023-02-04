#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

#include <cstdio>
#include <cstring>
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>


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


namespace CecilStLabs
{

   LocalShellBaseHandler::LocalShellBaseHandler( IConfig& config ) :
       m_configuration(config)
   {
      // intentionally left blank
   }

   LocalShellBaseHandler::~LocalShellBaseHandler()
   {
      // intentionally left blank
   }

   eShellHandlers LocalShellBaseHandler::processCommand( const std::string& command, const char delim, ICommandAction* action )
   {
      eShellHandlers retHandler = HANDLER_INVALID;
      std::vector<std::string> elems;

      split( command, delim, elems );

      if( elems.size() == 1 && elems[0] == "?" )
      {
         cmdHelp();
         retHandler = HANDLER_NORMAL;
      }
      else if( elems.size() == 1 && elems[0] == "license" )
      {
         cmdLicense();
         retHandler = HANDLER_NORMAL;
      }
      else if( elems.size() == 1 && elems[0] == "exit" )
      {
         retHandler = HANDLER_EXIT;
      }

      if( HANDLER_INVALID == retHandler )
      {
         std::cout << "Invalid command" << std::endl;
      }

      return retHandler;
   }

   void LocalShellBaseHandler::cmdHelp()
   {
      // TODO: can these be moved somewhere instead of hard coded?

      std::cout << "\nCommands available:\n";
      std::cout << "-------------------\n";
      std::cout << "  ?         - Show list of available commands\n";
      std::cout << "  license   - Show open source licensing\n";
      std::cout << "  exit      - Leave shell" << std::endl;
   }


   bool LocalShellBaseHandler::cmdLicense()
   {
      std::cout << std::endl << "TODO: Show opensource licenses here" << std::endl << std::endl;

      return true;
   }

   void LocalShellBaseHandler::split(const std::string& s, const char delim, std::vector<std::string> &elems)
   {
      std::stringstream ss(s);
      std::string item;
      while( getline( ss, item, delim ) )
      {
         elems.push_back(item);
      }
   }
}
