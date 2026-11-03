#include <CLI/Option.hpp>

#include <string>

namespace cli
{

Option::Name::Name():
	m_short_name(0)
{}

Option::Name::Name(char short_name):
	m_short_name(short_name)
{}

Option::Name::Name(const std::string& long_name):
	m_short_name(0),
	m_long_name(long_name)
{}

Option::Name::Name(char short_name, const std::string& long_name):
	m_short_name(short_name),
	m_long_name(long_name)
{}


char Option::Name::getShortName() const
{
	return m_short_name;
}

const std::string& Option::Name::getLongName() const
{
	return m_long_name;
}


bool Option::Name::operator==(const Option::Name& other) const
{
	return ((getShortName() == other.getShortName()) &&
			(getLongName()  == other.getLongName()));

	return false;
}

} //namespace app::cli
