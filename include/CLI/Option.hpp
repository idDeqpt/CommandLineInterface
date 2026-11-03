#ifndef OPTION_CLASS_HEADER
#define OPTION_CLASS_HEADER

#include <string>

namespace cli
{
    struct Option
    {
        enum Type
        {
            FLAG,
            ONE_ARGUMENT,
            MORE_ARGUMENTS
        };

        class Name
        {
        public:
            Name();
            Name(char short_name);
            Name(const std::string& long_name);
            Name(char short_name, const std::string& long_name);

            char getShortName() const;
            const std::string& getLongName() const;

            bool operator==(const Name& other) const;

        protected:
            char m_short_name;
            std::string m_long_name;
        };

        Name name;
        Type type;
    };
}

#endif //OPTION_CLASS_HEADER