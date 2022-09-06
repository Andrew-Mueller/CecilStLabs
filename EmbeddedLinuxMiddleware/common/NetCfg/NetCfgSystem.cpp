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

#include "NetCfgSystem.h"

namespace CecilStLabs
{

   NetCfgSystem::NetCfgSystem( IConfig& config ) :
       m_configuration(config)
   {
      // intentionally blank
   }

   NetCfgSystem::~NetCfgSystem()
   {
      // intentionally left blank
   }


   eNetCfgError NetCfgSystem::setIP( const string& ip, ICommandAction* action )
   {
      eNetCfgError cfgRet = NETCFG_SUCCESS;

      if( isValidIpAddress( ip ) )
      {
         // TODO: move these 'magic commands'.  LOL, they are like magic numbers
         //       except words instead of numbers.
         std::string cmd = "ifconfig eth0 " + ip + " netmask " + m_configuration.getValueByName( NetworkConfigSection::XMLTAG_SUBNET );

         // TODO: deal with the case where action-execute fails or produces and error?
         if (NULL != action)
         {
            action->execute( cmd.c_str() );

            m_configuration.setValueByName( NetworkConfigSection::XMLTAG_IP, ip );
         }

      }
      else
      {
         cfgRet = NETCFG_INVALID_IP;
      }

      return cfgRet;
   }

   eNetCfgError NetCfgSystem::setSubnet( const string& subnet, ICommandAction* action )
   {
      string cmd;
      eNetCfgError cfgRet = NETCFG_SUCCESS;

      if( isValidIpAddress( subnet ) )
      {
         cmd = "ifconfig eth0 " + m_configuration.getValueByName( NetworkConfigSection::XMLTAG_IP ) + " netmask " + subnet;

         if (NULL != action)
         {
            if (action->execute( cmd.c_str() < 0)
            {
               // TODO: deal with the case where action-execute fails or produces and error?
            }
         }

         m_configuration.setValueByName( NetworkConfigSection::XMLTAG_SUBNET, subnet );
      }
      else
      {
         cfgRet = NETCFG_INVALID_IP;
      }

      return cfgRet;

   }

   eNetCfgError NetCfgSystem::setGateway( const string& gateway, ICommandAction* action )
   {
      string cmd;
      eNetCfgError cfgRet = NETCFG_SUCCESS;

      if( isValidIpAddress( gateway ) )
      {
         cmd = "route add default gw " + gateway + " eth0";

         if (NULL != action)
         {
            if (0 > action->execute( cmd.c_str() ))
            {
               // TODO: deal with the failure of the route add.
            }
         }

         m_configuration.setValueByName( NetworkConfigSection::XMLTAG_GATEWAY, gateway );

      }
      else
      {
         cfgRet = NETCFG_INVALID_IP;
      }

      return cfgRet;
   }

   bool NetCfgSystem::isValidIpAddress( const string& ipAddress)
   {
      struct sockaddr_in sa;
      int result = inet_pton(AF_INET, ipAddress.c_str(), &(sa.sin_addr));
      return result != 0;
   }

}


