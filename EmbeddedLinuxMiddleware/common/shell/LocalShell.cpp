// TODO: this whole LocalShell system is in desperate need of cleanup and refactoring.

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#ifdef USE_PAM
#include <security/pam_appl.h>
#include <security/pam_misc.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
#include "LocalShellNormalHandler.h"
#include "LocalShellNetHandler.h"
#include "LocalShellNetDnsHandler.h"
#include "LocalShell.h"

namespace CecilStLabs
{
   // these need to be removed.
   const std::string LocalShell::LOCAL_USER = "LocalUser";
   const std::string LocalShell::LOCAL_PASSWORD = "password";
   const char LocalShell::DELIMS[] = {'\r', '\n'};

   #ifdef USE_PAM
      int shell_conv( int num_msg, const struct pam_message **msg, struct pam_response **resp, void *data );
   #endif

   void* shellTimeoutThread ( void *ptr )
   {
      bool* enterShell = (bool*)ptr;

      for (int i = LocalShell::SHELL_TIMEOUT_SECONDS; i > 0; i--)
      {
         // check flag for the 's' being entered
         if(true == *enterShell)
         {
            break;
         }

         sleep(1);
      }

      // if the 's' wasn't entered and it timed out, then exit the shell now
      if(false == *enterShell)
      {
         exit(0);
      }

      return NULL;
   }

   // BUG: members are not all properly initialized.  This needs to be looked
   //       at very closely since the bugs that arise from out of order variable
   //       initialization tend to look strange and are hard to track down.
   LocalShell::LocalShell( IConfig& config ) :
       m_pHandler(NULL),
       m_vHandlers(),
       m_enterShell(false),
       m_exit(false),
       m_curHandler(HANDLER_NORMAL),
       m_configuration(config)
   {

      m_vHandlers.push_back( new LocalShellNormalHandler( config ) );
      m_vHandlers.push_back( new LocalShellNetHandler( config ) );
      m_vHandlers.push_back( new LocalShellNetDnsHandler( config ) );

      m_pHandler = m_vHandlers[0];

      #ifdef USE_PAM
         m_conversation.conv = shell_conv;
         m_conversation.appdata_ptr = NULL;
      #endif
   }


   LocalShell::~LocalShell()
   {
      // delete all objects
      for( unsigned int i = 0; i < m_vHandlers.size(); i++ )
      {
         delete m_vHandlers[i];
      }
      m_vHandlers.clear();

   }

   bool LocalShell::executeIntro()
   {
      std::cout << "\nPress 'S' to enter local shell environment" << std::endl;
      
      m_enterShell = false;

      // start thread that will time out the shell and exit
      pthread_t timeoutThread;
      pthread_create( &timeoutThread, NULL, shellTimeoutThread, &m_enterShell );

      while(1)
      {
         char inputCharacter = getc( stdin );
         if( inputCharacter == 's' || inputCharacter == 'S')
         {
            m_enterShell = true;
            break;
         }
      }


      // clear stdin
      char inputCharacter;
      while (((inputCharacter = getchar()) != EOF) && (inputCharacter != '\n'));

      return m_enterShell;
   }

   void LocalShell::executeShell()
   {
      char user[MAX_USER_LENGTH];
      char *cmd;
      char line[MAX_LENGTH];
      eAuthCode authRet = AUTH_INVALID;
      ICommandAction* commandAction = new CommandActionSystem;


      std::cout << "Starting the LocalShell\n";
      std::cout << "-----------------------" << std::endl;

      while( authRet != AUTH_VALID )
      {
         // get user name
         printf( "Username: " );
         fgets(user, MAX_USER_LENGTH, stdin);

         // clear off the carriage return
         user[strlen(user)-1] = 0;

         if( !strcmp(user, "exit") )
         {
            break;
         }

         authRet = authenticate( user );
      }

      if( AUTH_VALID == authRet )
      {
         std::cout << "\n-----------------------------\n";
         std::cout << "Type '?' for list of commands\n";
         std::cout << "Type 'exit' to leave\n" << std::endl;

         while (1)
         {
            // show the prompt
            std::cout << m_pHandler->getPrompt();
            if( !fgets(line, MAX_LENGTH, stdin) )
            {
               break;
            }

            // Parse and execute command
            if( ( cmd = strtok(line, DELIMS) ) )
            {

               std::string str(cmd);
               eShellHandlers retHandler;

               retHandler = processHandler( str, commandAction );
               if( HANDLER_EXIT == retHandler )
               {
                  break;
               }
            }
         }
      }

   }

   eAuthCode LocalShell::authenticate( char* user )
   {
      eAuthCode retval = AUTH_INVALID;

      // look for hard-coded user first
      if( !strcmp( user, LOCAL_USER.c_str() ) )
      {
         char* p = getpass( "Password: " );
         if( !strcmp( p, LOCAL_PASSWORD.c_str()) )
         {
            retval = AUTH_VALID;
         }

      }

      // this is disgusting
      #ifdef USE_PAM
            else
            {
               pam_handle_t *pamh=NULL;
               int pamRet;

               // else use PAM to authenticate
               // TODO - only certain users? do we even need this PAM stuff?
               pamRet = pam_start( "other", user, &m_conversation, &pamh );

               if (pamRet == PAM_SUCCESS)
                  pamRet = pam_authenticate( pamh, 0 );

               pam_end( pamh, pamRet );

               if( PAM_SUCCESS == pamRet )
               {
                  retVal = AUTH_VALID;
               }
            }
      #endif

      return retval;
   }

   // If there is a derived class, make sure this gets called from overwritten function
   eShellHandlers LocalShell::processHandler( const std::string& command, ICommandAction* action )
   {
      eShellHandlers newHandler = m_pHandler->processCommand( command, ' ', action );

      if( newHandler != m_curHandler )
      {
         // handler changed, set up pointer
         for(unsigned int i = 0; i < m_vHandlers.size(); i++)
         {
            if( m_vHandlers[i]->getType() == newHandler )
            {
               m_pHandler = m_vHandlers[i];
               m_curHandler = newHandler;
               break;
            }
         }
      }

      return newHandler;
   }


   #ifdef USE_PAM
      int shell_conv( int num_msg, const struct pam_message **msg, struct pam_response **resp, void *data )
      {
         struct pam_response *aresp;
         char* p;

         if(( aresp = (pam_response*)calloc( num_msg, sizeof(struct pam_response ) ) ) == NULL )
            return (PAM_BUF_ERR);

         aresp[0].resp_retcode = 0;

         p = getpass("Password: ");
         aresp[0].resp = strdup(p);

         *resp = aresp;

         return (PAM_SUCCESS);
      }
   #endif
}

