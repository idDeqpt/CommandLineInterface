#include <iostream>
#include <CLI/Options.hpp>
#include <CLI/Option.hpp>
#include <CLI/Tools.hpp>


int main()
{
	int argc = 7;
	const char* argv[] = {
		"./dir",
		"-f",
		"-o",
		"option",
		"-m",
		"option1",
		"option2",
		"option3"
	};

	cli::Option::Name flag_option('f', "flag");
	cli::Option::Name one_option('o', "one");
	cli::Option::Name more_option('m', "more");

	std::vector<cli::Option> presets = {
		{flag_option, cli::Option::Type::FLAG},
		{one_option,  cli::Option::Type::ONE_ARGUMENT},
		{more_option, cli::Option::Type::MORE_ARGUMENTS}
	};

	cli::Options opt;
	std::cout << "Success: " << opt.parse(cli::tools::extractArguments(argv, argc), presets) << std::endl;

	bool flag = opt.has(flag_option);
	std::cout << "Flag: " << flag << std::endl;

	auto argument = opt.getArgument(one_option);
	if (argument != nullptr)
		std::cout << "One: " << *argument << std::endl;

	auto arguments = opt.getArguments(more_option);
	if (arguments != nullptr)
	{
		std::cout << "More:" << std::endl;
		for (const auto& str : *arguments)
			std::cout << "\t" << str << std::endl;
	}

	return 0;
}