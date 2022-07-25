#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>

// NOTE: that the C language include cstring is different than the c++
//       <string> library.
#include <cstring>

#include <iostream>
#include <sstream>
#include <string>
using namespace std;

#include "../common/util/BasicUtil.h"

#include "NetUtil.h"

namespace CecilStLabs
{

   NetUtil::NetUtil()
   {
      // intentionally left blank.

   }

   NetUtil::~NetUtil()
   {
      // intentionally left blank.
   }

   string NetUtil::HostnameToIP(string hostname)
   {
      string ip_address = "0.0.0.0";
      struct hostent* he;
      struct in_addr** addr_list;

      if (!BasicUtil::trim(hostname).empty())
      {
         he = gethostbyname(hostname.c_str());

         if (NULL == he)
         {
            // TODO: change this to the error logger instead of standard out.
            cout << "Error getting host by name" << endl;
         }
         else
         {
            char ip_addy[IP_ADDY_NUM_BYTES];
            memset(ip_addy, 0, IP_ADDY_NUM_BYTES);

            addr_list = (struct in_addr **) he->h_addr_list;

            for(uint8_t i = 0;
                ((i < IP_ADDY_NUM_BYTES) && (NULL != addr_list[i]));
                i++)
            {
               // Return the first address in the list.
               // TODO: why would more than one address be returned here?
               strcpy(ip_addy , inet_ntoa(*addr_list[i]) );
               ip_address = string(ip_addy);

               #ifdef DEBUG_PRINT
                  // TODO: change this to the error logger instead of just
                  //       standard out.
                  cout << "Address of the host: " << ip_address << endl;
               #endif

               break;
            }
         }
      }

      return ip_address;
   }
}
