#ifndef NETUTIL_H_
#define NETUTIL_H_

namespace CecilStLabs
{
   /**
    * Collection of common network utilities that are not owned by any
    * particular object.  These are similar to BasicUtil, but fit within the
    * Networking sub-category.
    */
   class NetUtil
   {
      public:

         /**
          * Create the Network Utility class.
          */
         NetUtil();

         /**
          * Destructor for the Network Utility class.
          */
         virtual ~NetUtil();

         /**
          * Look up the IP address of the specified hostname.  Retuns the host's
          * IP address as a string (containing dots and all as expected).
          *
          * @param hostname The host to lookup the IP address for.
          *        For example: www.CecilStLabs.com
          *
          * @return String containing the IP address of the host or an empty
          *         string if the host's IP could not be retrieved.
          */
         static std::string HostnameToIP(std::string hostname);

      protected:

         /**
          * The number of bytes that an IP address takes up.
          */
         static const uint8_t IP_ADDY_NUM_BYTES = 16;
   };
}
#endif
