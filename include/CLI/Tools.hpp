#ifndef COMMAND_LINE_OPTIONS_TOOLS_HEADER
#define COMMAND_LINE_OPTIONS_TOOLS_HEADER

#include <vector>
#include <string>

namespace cli::tools
{
	std::vector<std::string> extractArguments(const char* argv[], int argc);
}

#endif //COMMAND_LINE_OPTIONS_TOOLS_HEADER