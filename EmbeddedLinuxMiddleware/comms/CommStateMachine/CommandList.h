#ifndef COMMANDLIST_H
#define COMMANDLIST_H

namespace CecilStLabs
{
    class CommandList
    {
        public:
            CommandList() {}
            virtual ~CommandList() {}

        protected:
            CommandList(const CommandList& copy) {}
            CommandList& operator=(const CommandList& copy) { return *this; }

    };
}
#endif