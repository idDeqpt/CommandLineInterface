#include <CLI/Options.hpp>

#include <CLI/Option.hpp>
#include <algorithm>
#include <vector>
#include <string>
#include <map>

namespace cli
{

const std::string& Options::getCalledCommandName() const
{
	return m_command_name;
}

bool Options::hasArgument(const Option::Name& name) const
{
	for (const auto& opt : m_flags)
		if (opt == name) return true;

	for (const auto& opt : m_one_args)
		if (opt.first == name) return true;

	for (const auto& opt : m_more_args)
		if (opt.first == name) return true;

	return false;
}

const std::string* Options::getArgument(const Option::Name& name) const
{
	for (const auto& opt : m_one_args)
		if (opt.first == name) return &opt.second;
	return nullptr;
}

const std::vector<std::string>* Options::getArguments(const Option::Name& name) const
{
	for (const auto& opt : m_more_args)
		if (opt.first == name) return &opt.second;
	return nullptr;
}


bool Options::parse(const std::vector<std::string>& arguments, const std::vector<Command>& presets)
{
	m_command_name.clear();
	m_flags.clear();
	m_one_args.clear();
	m_more_args.clear();

	const Command* default_command = nullptr;
	std::map<std::string, const Command*> command_map;
	for (const auto& cmd : presets)
	{
		if (cmd.name.empty())
			default_command = &cmd;
		else
			command_map[cmd.name] = &cmd;
	}

	const Command* command = nullptr;
	unsigned int start_index = 0;

	if (!arguments.empty() && !arguments[0].empty() && arguments[0][0] != '-')
	{
		auto it = command_map.find(arguments[0]);
		if (it != command_map.end())
		{
			command = it->second;
			start_index = 1;
		}
		else
		{
			if (default_command == nullptr)
				return false;
			command = default_command;
		}
	}
	else
		command = default_command;

	if (command == nullptr)
		return false;

	m_command_name = command->name;

	std::map<std::string, Option> preset_map;
	for (const auto& opt : command->options)
	{
		const Option::Name& name = opt.name;
		Option::Type type = opt.type;

		if (name.getShortName() != 0)
			preset_map[std::string(1, name.getShortName())] = {name, type};
		if (!name.getLongName().empty())
			preset_map[name.getLongName()] = {name, type};
	}

	enum class State {NONE, COLLECT_MORE};
	State state = State::NONE;
	std::string token;
	std::vector<std::string> collected_args;
	Option::Name current_option;

	for (unsigned int i = start_index; i < arguments.size(); i++)
	{
		token = arguments[i];

		if (state == State::COLLECT_MORE)
		{
			if (!token.empty() && token[0] == '-')
			{
				if (collected_args.empty())
					return false;
				m_more_args.push_back({current_option, collected_args});
				collected_args.clear();
				state = State::NONE;
			}
			else
			{
				collected_args.push_back(token);
				continue;
			}
		}

		if (token.empty() || token[0] != '-')
			return false;

		if ((token.size() >= 2) && (token[1] == '-')) // --name or --name=value
		{
			std::string name_part = token.substr(2);
			std::string value;
			size_t equal_pos = name_part.find('=');
			if (equal_pos != std::string::npos)
			{
				value = name_part.substr(equal_pos + 1);
				name_part = name_part.substr(0, equal_pos);
			}
			auto it = preset_map.find(name_part);
			if (it == preset_map.end())
				return false;

			const auto& preset = it->second;
			const Option::Name& opt_name = preset.name;
			Option::Type        opt_type = preset.type;

			if (opt_type == Option::Type::FLAG)
				m_flags.push_back(opt_name);
			else if (opt_type == Option::Type::ONE_ARGUMENT)
			{
				if (!value.empty())
					m_one_args.push_back({opt_name, value});
				else
				{
					if ((i + 1) >= arguments.size())
						return false;

					std::string next_token = arguments[i + 1];
					if (!next_token.empty() && next_token[0] == '-')
						return false;

					m_one_args.push_back({opt_name, next_token});
					i++;
				}
			}
			else if (opt_type == Option::Type::MORE_ARGUMENTS)
			{
				state = State::COLLECT_MORE;
				current_option = opt_name;
				collected_args.clear();
				if (!value.empty())
					collected_args.push_back(value);
			}
		}
		else if (token.size() >= 2 && token[1] != '-') // -a or -abc
		{
			std::string opt_string = token.substr(1);

			if (opt_string.size() == 1) // -a
			{
				char c = opt_string[0];
				std::string key(1, c);
				auto it = preset_map.find(key);
				if (it == preset_map.end())
					return false;

				const auto& preset = it->second;
				const Option::Name& opt_name = preset.name;
				Option::Type        opt_type = preset.type;

				if (opt_type == Option::Type::FLAG)
					m_flags.push_back(opt_name);
				else if (opt_type == Option::Type::ONE_ARGUMENT)
				{
					if ((i + 1) >= arguments.size())
						return false;

					std::string next_token = arguments[i + 1];
					if (!next_token.empty() && next_token[0] == '-')
						return false;

					m_one_args.push_back({opt_name, next_token});
					i++;
				}
				else if (opt_type == Option::Type::MORE_ARGUMENTS)
				{
					state = State::COLLECT_MORE;
					current_option = opt_name;
					collected_args.clear();
				}
			}
			else // -abc, only flags
			{
				for (char c : opt_string)
				{
					std::string key(1, c);
					auto it = preset_map.find(key);
					if (it == preset_map.end())
						return false;

					const auto& preset = it->second;
					if (preset.type != Option::Type::FLAG)
						return false;
					m_flags.push_back(preset.name);
				}
			}
		}
		else //token starts on '-', but not State::COLLECT_MORE
			return false;
	}

	if (state == State::COLLECT_MORE)
	{
		if (collected_args.empty())
			return false;
		m_more_args.push_back({current_option, collected_args});
	}

	return true;
}

} //namespace app::cli
