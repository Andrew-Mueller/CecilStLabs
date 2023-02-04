#ifndef EMPTYLOGDRIVER_H_
#define EMPTYLOGDRIVER_H_

namespace CecilStLabs
{

   /**
    * Create a log driver that has empty implementations of its required virtual
    * methods.  This allows a zero impact log driver to be provided for example
    * by default to ensure null references are not possible when a logger might
    * be expected or needed.
    */
   class emptyLogDriver : public ILogDriver
   {
      public:
         emptyLogDriver();
         virtual ~emptyLogDriver();

         void Log(std::string message,
                  eLogLevel level,
                  std::string type = "");

         void log(std::string message,
                  eLogLevel level,
                  std::string type = "",
                  eLogSource source = LogSourceIrregardless);

         LogEntry Parse(std::string message);
   };

}

#endif
