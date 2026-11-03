#ifndef COMMAND_LINE_OPTIONS_CLASS_HEADER
#define COMMAND_LINE_OPTIONS_CLASS_HEADER

#include <CLI/Command.hpp>
#include <CLI/Option.hpp>
#include <vector>
#include <string>

namespace cli
{
	class Options
	{
	public:
		using FlagOption     = Option::Name;
		using OneArgOption   = std::pair<Option::Name, std::string>;
		using MoreArgsOption = std::pair<Option::Name, std::vector<std::string>>;

		const std::string&              getCalledCommandName() const;
		bool                            hasArgument(const Option::Name& name) const;
		const std::string*              getArgument(const Option::Name& name) const;
		const std::vector<std::string>* getArguments(const Option::Name& name) const;

		bool parse(const std::vector<std::string>& arguments, const std::vector<Command>& presets);

	protected:
		std::string m_command_name;
		std::vector<FlagOption>     m_flags;
		std::vector<OneArgOption>   m_one_args;
		std::vector<MoreArgsOption> m_more_args;
	};
}

#endif //COMMAND_LINE_OPTIONS_CLASS_HEADER