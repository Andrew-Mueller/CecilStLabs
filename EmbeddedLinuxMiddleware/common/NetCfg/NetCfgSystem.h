#ifndef NETCFGSYSTEM_H_
#define NETCFGSYSTEM_H_

namespace CecilStLabs
{
   enum eNetCfgError
   {
      NETCFG_SUCCESS = 0,
      NETCFG_NOT_PROCESSED,
      NETCFG_INVALID_IP,
      NETCFG_SYSTEM,
      NETCFG_FILEIO
   };

   /**
    * Object used to set the system's network configuration such as the IP
    * address, gateway, etc.
    */
   class NetCfgSystem
   {
      public:
         /**
          * Create the network config system.
          *
          * @param config the config section for the common network parameters
          */
         NetCfgSystem( IConfig& config );

         /**
          * release resources used by the network config system object.
          */
         virtual ~NetCfgSystem();

         /**
          * set the IP address
          *
          * @param ip string representation of the ip address in the format of "192.168.1.1"
          * @param action The specific command or action to use to set the IP
          *               address for the system for example: a system call
          *
          * @return true if address is valid and is successfully set
          */
         eNetCfgError setIP( const string& ip, ICommandAction* action );

         /**
          * set the subnet address
          *
          * @param subnet  string to set
          * @param action  the type of action like a system call
          *
          * @return true if address is valid and is successfully set
          */
         eNetCfgError setSubnet( const string& subnet, ICommandAction* action );

         /**
          * set the gateway address
          *
          * @param gateway string to set
          * @param action  the type of action like a system call
          *
          * @return true if address is valid and is successfully set
         */
         eNetCfgError setGateway( const string& gateway, ICommandAction* action );

   private:
         /**
          * configuration object containing the network configuration.
          */
         IConfig& m_configuration;

         /**
          * Checks if an IP address is formatted properly
          *
          * @param ipAddress ip address string
          *
          * @return true if the IP address is valid, false if not
          */
         bool isValidIpAddress( const string& ipAddress);

   };
}
#endif
