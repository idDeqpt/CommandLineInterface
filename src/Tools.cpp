#include <CLI/Tools.hpp>

#include <vector>
#include <string>

namespace cli::tools
{

std::vector<std::string> extractArguments(const char* argv[], int argc)
{
	std::vector<std::string> arguments;
	arguments.reserve(argc - 1);

	for (unsigned int i = 1; i < argc; i++)
		arguments.push_back(argv[i]);

	return std::move(arguments);
}

} //namespace cli::tools
