#include <iostream>
#include <string>
#include <vector>

#include <CLI/Options.hpp>
#include <CLI/Option.hpp>
#include <CLI/Tools.hpp>

static int g_failures = 0;

static void check(bool cond, const std::string& label)
{
    std::cout << (cond ? "[ OK ] " : "[FAIL] ") << label << "\n";
    if (!cond) ++g_failures;
}

int main()
{
    // ============================================================
    // 1. Команда по умолчанию ("прямой" вызов без подкоманды)
    // ============================================================
    {
        std::cout << "\n--- Scenario 1: default command ---\n";

        int argc = 8;
        const char* argv[] = {
            "./dir",
            "-f",
            "-o", "option",
            "-m", "option1", "option2", "option3"
        };

        cli::Option::Name flag_option('f', "flag");
        cli::Option::Name one_option('o', "one");
        cli::Option::Name more_option('m', "more");

        std::vector<cli::Command> presets = {
            { "", {
                {flag_option, cli::Option::Type::FLAG},
                {one_option,  cli::Option::Type::ONE_ARGUMENT},
                {more_option, cli::Option::Type::MORE_ARGUMENTS}
            }}
        };

        cli::Options opt;
        check(opt.parse(cli::tools::extractArguments(argv, argc), presets),
              "parse succeeded");

        check(opt.getCalledCommandName().empty(),
              "called command name is empty (no subcommand)");

        check(opt.hasArgument(flag_option), "flag -f present");
        check(opt.hasArgument(one_option),  "one  -o present (hasArgument)");
        check(opt.hasArgument(more_option), "more -m present (hasArgument)");

        const std::string* one = opt.getArgument(one_option);
        check(one != nullptr && *one == "option", "one == \"option\"");

        const std::vector<std::string>* more = opt.getArguments(more_option);
        check(more != nullptr && more->size() == 3, "more has 3 values");
        if (more && more->size() == 3)
        {
            check((*more)[0] == "option1", "more[0] == option1");
            check((*more)[1] == "option2", "more[1] == option2");
            check((*more)[2] == "option3", "more[2] == option3");
        }
    }

    // ============================================================
    // 2. Явная подкоманда "serve" (плюс команда по умолчанию)
    // ============================================================
    {
        std::cout << "\n--- Scenario 2: explicit command 'serve' ---\n";

        int argc = 8;
        const char* argv[] = {
            "./dir",
            "serve",
            "--port", "8080",
            "-r", "res/a", "res/b",
            "-v"
        };

        cli::Option::Name flag_option('f', "flag");
        cli::Option::Name port_option('p', "port");
        cli::Option::Name resources_option('r', "resources");
        cli::Option::Name verbose_option('v', "verbose");

        std::vector<cli::Command> presets = {
            { "", {
                {flag_option, cli::Option::Type::FLAG}
            }},
            { "serve", {
                {port_option,      cli::Option::Type::ONE_ARGUMENT},
                {resources_option, cli::Option::Type::MORE_ARGUMENTS},
                {verbose_option,   cli::Option::Type::FLAG}
            }}
        };

        cli::Options opt;
        check(opt.parse(cli::tools::extractArguments(argv, argc), presets),
              "parse succeeded");

        check(opt.getCalledCommandName() == "serve",
              "called command == \"serve\"");

        check(opt.hasArgument(verbose_option), "verbose flag present");
        check(!opt.hasArgument(flag_option),   "flag from default is NOT present");

        const std::string* port = opt.getArgument(port_option);
        check(port != nullptr && *port == "8080", "port == 8080");

        const std::vector<std::string>* res = opt.getArguments(resources_option);
        check(res != nullptr && res->size() == 2, "resources has 2 values");
        if (res && res->size() == 2)
        {
            check((*res)[0] == "res/a", "resources[0] == res/a");
            check((*res)[1] == "res/b", "resources[1] == res/b");
        }
    }

    // ============================================================
    // 3. Форма --name=value
    // ============================================================
    {
        std::cout << "\n--- Scenario 3: --name=value form ---\n";

        int argc = 4;
        const char* argv[] = {
            "./dir",
            "--port=9000",
            "-f",
            "--port=9001"
        };

        cli::Option::Name flag_option('f', "flag");
        cli::Option::Name port_option('p', "port");

        std::vector<cli::Command> presets = {
            { "", {
                {flag_option, cli::Option::Type::FLAG},
                {port_option, cli::Option::Type::ONE_ARGUMENT}
            }}
        };

        cli::Options opt;
        check(opt.parse(cli::tools::extractArguments(argv, argc), presets),
              "parse succeeded");
        check(opt.getCalledCommandName().empty(),
              "called command name is empty");

        check(opt.hasArgument(flag_option), "flag present");
        // getArgument возвращает первое вхождение
        const std::string* port = opt.getArgument(port_option);
        check(port != nullptr && *port == "9000",
              "port == 9000 (first occurrence)");
    }

    // ============================================================
    // 4. Подкоманда без опций: "build"
    // ============================================================
    {
        std::cout << "\n--- Scenario 4: command without arguments ---\n";

        int argc = 2;
        const char* argv[] = { "./dir", "build" };

        cli::Option::Name flag_option('f', "flag");

        std::vector<cli::Command> presets = {
            { "", { {flag_option, cli::Option::Type::FLAG} } },
            { "build", {} }
        };

        cli::Options opt;
        check(opt.parse(cli::tools::extractArguments(argv, argc), presets),
              "parse succeeded");
        check(opt.getCalledCommandName() == "build",
              "called command == \"build\"");
        check(!opt.hasArgument(flag_option),
              "no options are set");
        check(opt.getArgument(flag_option) == nullptr,
              "getArgument returns nullptr");
        check(opt.getArguments(flag_option) == nullptr,
              "getArguments returns nullptr");
    }

    // ============================================================
    // 5. Неизвестная команда, команды по умолчанию нет -> false
    // ============================================================
    {
        std::cout << "\n--- Scenario 5: unknown command, no default ---\n";

        int argc = 3;
        const char* argv[] = { "./dir", "unknown", "-f" };

        cli::Option::Name flag_option('f', "flag");

        std::vector<cli::Command> presets = {
            { "serve", { {flag_option, cli::Option::Type::FLAG} } }
        };

        cli::Options opt;
        check(!opt.parse(cli::tools::extractArguments(argv, argc), presets),
              "parse failed (unknown command, no default)");
        check(opt.getCalledCommandName().empty(),
              "command name cleared after failed parse");
    }

    // ============================================================
    // 6. Неизвестный первый токен при наличии default -> false
    //    (парсер не принимает позиционные аргументы вне команды)
    // ============================================================
    {
        std::cout << "\n--- Scenario 6: non-option token with default ---\n";

        int argc = 3;
        const char* argv[] = { "./dir", "positional", "-f" };

        cli::Option::Name flag_option('f', "flag");

        std::vector<cli::Command> presets = {
            { "", { {flag_option, cli::Option::Type::FLAG} } }
        };

        cli::Options opt;
        check(!opt.parse(cli::tools::extractArguments(argv, argc), presets),
              "parse failed (positional token not supported)");
    }

    // ============================================================
    // 7. Пустой argv: вызов программы без аргументов
    // ============================================================
    {
        std::cout << "\n--- Scenario 7: no arguments ---\n";

        int argc = 1;
        const char* argv[] = { "./dir" };

        cli::Option::Name flag_option('f', "flag");

        std::vector<cli::Command> presets = {
            { "", { {flag_option, cli::Option::Type::FLAG} } }
        };

        cli::Options opt;
        check(opt.parse(cli::tools::extractArguments(argv, argc), presets),
              "parse succeeded (default command, no options)");
        check(opt.getCalledCommandName().empty(),
              "called command name is empty");
        check(!opt.hasArgument(flag_option), "flag not present");
    }

    std::cout << "\n=================================\n";
    std::cout << "Failures: " << g_failures << "\n";
    return g_failures == 0 ? 0 : 1;
}