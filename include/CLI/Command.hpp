#ifndef COMMAND_CLASS_HEADER
#define COMMAND_CLASS_HEADER

#include <CLI/Option.hpp>
#include <string>
#include <vector>

namespace cli
{
    struct Command
    {
        std::string name;
        std::vector<Option> options;
    };
}

#endif //COMMAND_CLASS_HEADER