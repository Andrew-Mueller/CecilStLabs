#ifndef ENUMBYNAME_H_
#define ENUMBYNAME_H_

namespace CecilStLabs
{

   /**
    * Create a natural mapping between an enumeration and a string that might be
    * used to define it in a configuration or other text based interpretation.
    */
   class EnumByName
   {
      public:

         /**
          * Create the mapping between the strongly typed enumeration and the
          * string name.
          *
          * @param type The value of the enumeration type to map to the specified name.
          * @param name The string representation of the enumeration value specified.
          */
         EnumByName(uint32_t type, std::string name);

         /**
          * release the resources consumed by the object.
          */
         virtual ~EnumByName();

         /**
          * checks the specified string against this types string for a possible match.
          * Matches the name independent of case-sensitivity.
          *
          * @param name String to check against this enumerations designated name.
          *
          * @return True if the name matches, False if it does not.
          */
         bool isEnumType(std::string name) const;

         /**
          * getter for the type associated with this object's name.
          */
         inline uint32_t getType() const { return m_type; };
         inline std::string getName() const { return m_name; };

      protected:

         /**
          * The strongly typed enumeration intended for use with the above string.
          *
          * enumerations are not portable as a generic type.  Since we are
          * targeting 32 bit micros we can safely cast them to an unsigned 32
          * bit integer.
          */
         uint32_t m_type;

         /**
          * String name of the enumeration.
          */
         std::string m_name;
   };

}
#endif
