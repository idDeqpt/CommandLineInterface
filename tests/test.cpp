#include <iostream>
#include <CLI/CommandLineOptions.hpp>


std::vector<std::string> convert(const char* argv[], int argc)
{
	std::vector<std::string> args;
	args.reserve(argc - 1);

	for (unsigned int i = 1; i < argc; i++)
		args.push_back(argv[i]);

	return std::move(args);
}


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

	cli::OptionName flag_option('f', "flag");
	cli::OptionName one_option('o', "one");
	cli::OptionName more_option('m', "more");

	cli::Options opt;

	std::cout << "Argc" << std::endl;
	std::cout << "Success: " << opt.parse(argv, argc, {
		{flag_option, cli::OptionType::FLAG},
		{one_option,  cli::OptionType::ONE_ARGUMENT},
		{more_option, cli::OptionType::MORE_ARGUMENTS}
	}) << std::endl;

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

	std::cout << "Vector" << std::endl;
	auto args = convert(argv, argc);
	std::cout << "Success: " << opt.parse(args, {
		{flag_option, cli::OptionType::FLAG},
		{one_option,  cli::OptionType::ONE_ARGUMENT},
		{more_option, cli::OptionType::MORE_ARGUMENTS}
	}) << std::endl;

	flag = opt.has(flag_option);
	std::cout << "Flag: " << flag << std::endl;

	argument = opt.getArgument(one_option);
	if (argument != nullptr)
		std::cout << "One: " << *argument << std::endl;

	arguments = opt.getArguments(more_option);
	if (arguments != nullptr)
	{
		std::cout << "More:" << std::endl;
		for (const auto& str : *arguments)
			std::cout << "\t" << str << std::endl;
	}

	return 0;
}