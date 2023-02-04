#ifndef ILOGGABLE_H
#define ILOGGABLE_H

namespace CecilStLabs
{
   /**
    * Interface to specify an object that has access to writing log messages.
    * A class will implement this interface if they desire to write messages
    * to the systems log.  This keeps the ownership of the logger in a single place
    * and saves each object that wants to write to the log from redundantly passing
    * an ILogDriver at construction time or some other time.
    */
   class ILoggable
   {
      public:

         /**
          * Sets the log driver to the one specified.
          *
          * @param logDriver The concrete log driver to use for logging.
          */
         inline void setLogDriver(ILogDriver* logDriver) { m_logDriver = logDriver; };

         /**
          * Retrieves the log driver currently being used.
          *
          * @return Pointer to the ILogDriver being used by this loggable
          *         interface.  Ensures a null pointer cannot be returned by
          *         returning the empty log driver has not properly been set.
          */
         inline ILogDriver* getLogDriver()
         {
            if (NULL == m_logDriver)
            {
               return &m_emptyLogDriver;
            }
            else
            {
               return m_logDriver;
            }
         };

      protected:

         // protected constructor, copy constructor, assignment operator, and
         // destructor implementation to stop accidental creation of an instance
         // of this interface.
         ILoggable() : m_logDriver(&m_emptyLogDriver) {};
         ILoggable(ILoggable* copy __attribute__((unused))) : m_logDriver(&m_emptyLogDriver) {};
         virtual ~ILoggable() {};

         ILoggable* operator=(ILoggable* rhs __attribute__((unused))) { return rhs; };

         /**
          * Reference to the ILogDriver that will be used for logging.
          */
         ILogDriver* m_logDriver;

         /**
          * This is an empty log driver to make sure the pointer to the log
          * driver points to "something" even if it is this empty log driver.
          */
         emptyLogDriver m_emptyLogDriver;
   };

}

#endif
