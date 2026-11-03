#ifndef COMMAND_LINE_OPTIONS_CLASS_HEADER
#define COMMAND_LINE_OPTIONS_CLASS_HEADER

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

		bool has(const Option::Name& name) const;

		const std::string*              getArgument(const Option::Name& name) const;
		const std::vector<std::string>* getArguments(const Option::Name& name) const;

		const std::vector<FlagOption>&     getFlags()    const;
		const std::vector<OneArgOption>&   getOneArgs()  const;
		const std::vector<MoreArgsOption>& getMoreArgs() const;

		bool parse(const char* argv[], int argc, const std::vector<Option>& presets); //with path
		bool parse(const std::vector<std::string>& arguments, const std::vector<Option>& presets); //without path

	protected:
		std::vector<FlagOption>     m_flags;
		std::vector<OneArgOption>   m_one_args;
		std::vector<MoreArgsOption> m_more_args;
	};
}

#endif //COMMAND_LINE_OPTIONS_CLASS_HEADER