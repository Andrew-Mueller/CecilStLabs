#ifndef MESSAGEPARSER_H
#define MESSAGEPARSER_H

namespace CecilStLabs
{
    /**
     * Parser for the HTTP Message Body into an application layer message
     * 
     */
    class MessageParser : public ILoggable
    {
        public:
            MessageParser(CommandList* commandList,
                          IClockDriver* clockDriver,
                          IConfig& wholeConfig,
                          std::string accessKey,
                          std::string logicalDeviceID);
                            
            virtual ~MessageParser();

            void parse(uint32_t id, std::string message);

        protected:

    };
}

#endif
