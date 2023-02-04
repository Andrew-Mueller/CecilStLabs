#ifndef NAMEVALUEPAIR_H_
#define NAMEVALUEPAIR_H_

namespace CecilStLabs
{

   /**
    * A general name and value pair used for such things as getting and setting
    * configuration parameters.
    */
   class NameValuePair : public SingleLink
   {
      public:

         /**
          * default constructor for the name and value pair.  Used by the
          * memory pool.
          */
         NameValuePair();

         /**
          * Create a name and value pair with the specified name and value.
          */
         NameValuePair(std::string name, std::string value);

         /**
          * release any resources consumed by the object.
          */
         virtual ~NameValuePair();

         /**
          * Set the name of the name value pair.
          * @param name The name of the name and value pair.
          */
         inline void setName(std::string name) { m_name.assign(name); };

         /**
          * Get the name of the name value pair.
          * @return The name for the name value pair.
          */
         inline std::string getName() { return m_name; };

         /**
          * Set the value of the name value pair.
          * @param The value for the name value pair.
          */
         inline void setValue(std::string value) {m_value.assign(value); };

         /**
          * Get the value of the name value pair.
          * @return retrieve the value of the name value pair.
          */
         inline std::string getValue() { return m_value; };

      protected:

         // NOTE: define the copy constructor so the compiler doesn't generate it for us.
         NameValuePair(NameValuePair& copy __attribute__((unused))) : SingleLink((SingleLink&)copy) {};

         /**
          * The name for our name and value pair.
          */
         std::string m_name;

         /**
          * The value associated with the name.
          */
         std::string m_value;
   };

}
#endif
