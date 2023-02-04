// TODO: this whole LocalShell system is in desperate need of cleanup and refactoring.

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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


#include "../../common/NetCfg/ICommandAction.h"
#include "../../common/NetCfg/CommandActionSystem.h"

#include "LocalShellBaseHandler.h"
#include "LocalShellNetDnsHandler.h"


namespace CecilStLabs
{
   const string LocalShellNetDnsHandler::SHELL_DNS_PROMPT = "net dns $ ";
   const string LocalShellNetDnsHandler::SHELL_DNS_SHOW = "show";
   const string LocalShellNetDnsHandler::SHELL_DNS_REMOVE = "remove";
   const string LocalShellNetDnsHandler::SHELL_DNS_ADD = "add";
   const string LocalShellNetDnsHandler::SHELL_DNS_BACK = "..";

   LocalShellNetDnsHandler::LocalShellNetDnsHandler( IConfig& config ) :
       LocalShellBaseHandler(config)
   {
      m_type = HANDLER_NET_DNS;
      m_prompt = SHELL_DNS_PROMPT;
   }

   LocalShellNetDnsHandler::~LocalShellNetDnsHandler()
   {
   }


   // If there is a derived class, make sure this gets called from overwritten function
   eShellHandlers LocalShellNetDnsHandler::processCommand( const string& command, const char delim, ICommandAction* action )
   {

      eShellHandlers retHandler = HANDLER_INVALID;
      vector<string> elems;

      split( command, delim, elems );

      if( elems.size() == 1 && elems[0] == SHELL_DNS_SHOW )
      {
         cmdShow( command, action );
         retHandler = HANDLER_NET_DNS;
      }
      else if( elems.size() >= 1 && elems[0] == SHELL_DNS_REMOVE )
      {
         cmdRemove( command, action );
         retHandler = HANDLER_NET_DNS;
      }
      else if( elems.size() >= 1 && elems[0] == SHELL_DNS_ADD )
      {
         cmdAdd( command, action );
         retHandler = HANDLER_NET_DNS;
      }
      else if( elems.size() == 1 && elems[0] == SHELL_DNS_BACK )
      {
         retHandler = HANDLER_NET;
      }

      if( HANDLER_INVALID == retHandler )
      {
         retHandler = LocalShellBaseHandler::processCommand( command, delim, action );
         if( HANDLER_NORMAL == retHandler )
         {
            retHandler = HANDLER_NET_DNS;
         }
      }

      return retHandler;
   }

   // If there is a derived class, make sure this gets called first from overwritten function
   void LocalShellNetDnsHandler::cmdHelp()
   {
      LocalShellBaseHandler::cmdHelp();

      cout << "  show          - Show DNS addresses." << endl;
      cout << "  remove <x>    - Remove a DNS address." << endl;
      cout << "  add <x.x.x.x> - Set a new DNS address" << endl;

   }

   void LocalShellNetDnsHandler::cmdShow( const string& command,ICommandAction* action )
   {
      //TODO
   }

   void LocalShellNetDnsHandler::cmdRemove( const string& command, ICommandAction* action )
   {
      //TODO
   }

   void LocalShellNetDnsHandler::cmdAdd( const string& command, ICommandAction* action )
   {
      //TODO
   }

}


