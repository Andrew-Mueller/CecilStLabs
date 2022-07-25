// TODO: this whole LocalShell system is in desperate need of cleanup and refactoring.

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
#include "LocalShellNetHandler.h"

namespace CecilStLabs
{

   LocalShellNetHandler::LocalShellNetHandler( IConfig& config ) :
       LocalShellBaseHandler(config)
   {
      m_type = HANDLER_NET;
      m_prompt = "net$ ";
   }

   LocalShellNetHandler::~LocalShellNetHandler()
   {
      // intentionally left blank
   }

   eShellHandlers LocalShellNetHandler::processCommand( const string& command, const char delim, ICommandAction* action )
   {
      eShellHandlers retHandler = HANDLER_INVALID;
      vector<string> elems;

      split( command, delim, elems );
      if( elems.size() >= 1 )
      {
         if( 0 == strncasecmp(NetworkConfigSection::XMLTAG_IP.c_str(), elems[0].c_str(), NetworkConfigSection::XMLTAG_IP.length()) )
         {
            cmdIP( command, delim, action );
            retHandler = HANDLER_NET;
         }
         else if( 0 == strncasecmp(NetworkConfigSection::XMLTAG_SUBNET.c_str(), elems[0].c_str(), NetworkConfigSection::XMLTAG_SUBNET.length()) )
         {
            cmdSubnet( command, delim, action );
            retHandler = HANDLER_NET;
         }
         else if( 0 == strncasecmp(NetworkConfigSection::XMLTAG_GATEWAY.c_str(), elems[0].c_str(), NetworkConfigSection::XMLTAG_GATEWAY.length()) )
         {
            cmdGateway( command, delim, action );
            retHandler = HANDLER_NET;
         }
         else if( 0 == strncasecmp(NetworkConfigSection::XMLTAG_DNS.c_str(), elems[0].c_str(), NetworkConfigSection::XMLTAG_DNS.length()) )
         {
            retHandler = HANDLER_NET_DNS;
         }
         else if( 0 == strncasecmp(NetworkConfigSection::XMLTAG_ACCESS_KEY.c_str(), elems[0].c_str(), NetworkConfigSection::XMLTAG_ACCESS_KEY.length()) )
         {
            cmdAccessKey( command, delim, action );
            retHandler = HANDLER_NET;
         }
         else if( 0 == strncasecmp(NetworkConfigSection::XMLTAG_DEVICE_SERIAL_NUMBER.c_str(), elems[0].c_str(), NetworkConfigSection::XMLTAG_DEVICE_SERIAL_NUMBER.length()) )
         {
            cmdDeviceId(command, delim, action);
            retHandler = HANDLER_NET;
         }
         else if( elems.size() == 1 && elems[0] == ".." )
         {
            retHandler = HANDLER_NORMAL;
         }
      }

      if( HANDLER_INVALID == retHandler )
      {
         retHandler = LocalShellBaseHandler::processCommand( command, delim, action );
         if( HANDLER_NORMAL == retHandler )
         {
            retHandler = HANDLER_NET;
         }
      }

      return retHandler;
   }

   void LocalShellNetHandler::cmdHelp()
   {
      LocalShellBaseHandler::cmdHelp();

      cout << "  " << NetworkConfigSection::XMLTAG_IP << " - Set or show IP address.             Use ip <x.x.x.x> to set" << endl;
      cout << "  " << NetworkConfigSection::XMLTAG_SUBNET << " - Set or show subnet address.         Use subnet <x.x.x.x> to set" << endl;
      cout << "  " << NetworkConfigSection::XMLTAG_GATEWAY << " - Set or show gateway address.        Use gateway <x.x.x.x> to set" << endl;
      cout << "  " << NetworkConfigSection::XMLTAG_DNS << " - Enter dns entry submenu" << endl;
      cout << "  " << NetworkConfigSection::XMLTAG_ACCESS_KEY << " - Set or show the access key.         Use accesskey <key> to set" << endl;
      cout << "  " << NetworkConfigSection::XMLTAG_DEVICE_SERIAL_NUMBER << " - Set or show the device serial number.  Use deviceid <id> to set" << endl;
      cout << "  ..        - Back out of net menu." << endl;
   }

   void LocalShellNetHandler::cmdIP( const string& command, const char delim, ICommandAction* action )
   {
      vector<string> elems;
      string cmd;

      split( command, delim, elems );

      if( elems.size() == 1 )
      {
         cout << endl << "IP address: " <<  m_configuration.getValueByName( elems[0] ) << endl << endl;
      }
      else if( elems.size() == 2  )
      {
         if( isValidIpAddress( elems[1] ) )
         {
            cout << endl << "New IP address: " <<  elems[1] << endl << endl;

            cmd = "ifconfig eth0 " + elems[1] + " netmask " + m_configuration.getValueByName( NetworkConfigSection::XMLTAG_SUBNET );

            // TODO - failure?
            action->execute( cmd.c_str() );

            m_configuration.setValueByName( NetworkConfigSection::XMLTAG_IP, elems[1] );
         }
         else
         {
            // TODO - bad ip address?
         }

      }
      else
      {
         cout << endl << "Invalid ip command" << endl;
      }
   }

   void LocalShellNetHandler::cmdSubnet( const string& command, const char delim, ICommandAction* action )
   {
      vector<string> elems;
      string cmd;

      split( command, delim, elems );

      if( elems.size() == 1 )
      {
         cout << endl << "Subnet mask: " <<  m_configuration.getValueByName( elems[0] ) << endl << endl;
      }
      else if( elems.size() == 2 )
      {
         if( isValidIpAddress( elems[1] ) )
         {
            cout << endl << "New subnet: " <<  elems[1] << endl << endl;

            cmd = "ifconfig eth0 " + m_configuration.getValueByName( NetworkConfigSection::XMLTAG_IP ) + " netmask " + elems[1];

            //TODO - failure?
            action->execute( cmd.c_str() );

            m_configuration.setValueByName( NetworkConfigSection::XMLTAG_SUBNET, elems[1] );
         }
         else
         {
            // TODO - bad ip address?
         }
      }
      else
      {
         cout << endl << "Invalid subnet command" << endl;
      }

   }

   void LocalShellNetHandler::cmdGateway( const string& command, const char delim, ICommandAction* action )
   {
      vector<string> elems;
      string cmd;

      split( command, delim, elems );

      if( elems.size() == 1 )
      {
         cout << endl << "Gateway address: " <<  m_configuration.getValueByName( elems[0] ) << endl << endl;
      }
      else if( elems.size() == 2 )
      {

         if( isValidIpAddress( elems[1] ) )
         {
            cout << endl << "New gateway: " <<  elems[1] << endl << endl;

            cmd = "route add default gw " + elems[1] + " eth0";

            // TODO - failure?
            action->execute( cmd.c_str() );

            m_configuration.setValueByName( NetworkConfigSection::XMLTAG_GATEWAY, elems[1] );
         }
         else
         {
            // TODO - bad ip address?
         }

      }
      else
      {
         cout << endl << "Invalid gateway command" << endl;
      }

   }

   void LocalShellNetHandler::cmdAccessKey( const string& command, const char delim, ICommandAction* action )
   {
      vector<string> elems;

      split( command, delim, elems );

      if( elems.size() == 1 )
      {
         cout << endl << "Access Key: " <<  m_configuration.getValueByName( elems[0] ) << endl << endl;
      }
      else if( elems.size() == 2 )
      {
         cout << endl << "New Access Key: " <<  elems[1] << endl << endl;

         m_configuration.setValueByName( elems[0], elems[1] );
      }
      else
      {
         cout << endl << "Invalid accesskey command" << endl;
      }

   }

   void LocalShellNetHandler::cmdDeviceId( const string& command, const char delim, ICommandAction* action )
   {
      vector<string> elems;

      split( command, delim, elems );

      if( elems.size() == 1 )
      {
         cout << endl << "Logical Device ID: : " <<  m_configuration.getValueByName( elems[0] ) << endl << endl;
      }
      else if( elems.size() == 2 )
      {
         cout << endl << "New URL: " <<  elems[1] << endl << endl;

         m_configuration.setValueByName( elems[0], elems[1] );
      }
      else
      {
         cout << endl << "Invalid deviceid command" << endl;
      }

   }

   bool LocalShellNetHandler::isValidIpAddress( const string& ipAddress)
   {
      struct sockaddr_in sa;
      int result = inet_pton(AF_INET, ipAddress.c_str(), &(sa.sin_addr));
      return result != 0;
   }

}


