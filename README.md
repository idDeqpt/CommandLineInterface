# CommandLineInterface

C++ библиотека для взаимодействия с командной строкой. Предоставляет функционал для парсинга аргументов из argc и argv (стандартных переменных main).
____
# Требования
Основные требования:
- CMake 3.14+
- C++17-совместимый компилятор
___
# Сборка
## Windows
```bash
# Вариант 1: Использовать готовый скрипт
GenerateRelease.bat
# Вариант 2: Вручную
mkdir build && cd build
cmake ..
cmake --build . --config Release
```
___
# Использование
Форматы имён ключей:
| Формат   | Пример |
| :------- | :----- |
| Короткий | -p     |
| Длинный  | --port |

Для одного ключа можно задать как короткий, так и длинный варианты имени.

Форматы видов ключей:
| Формат                     | Описание |
| :------------------------- | :----- |
| Флаг                       | Задаётся без аргументов |
| С одним аргументом         | Задаётся с одним аргументом, следующим строго после ключа |
| С несколькими аргументами  | Задаётся с множеством аргументов, следующими строго после ключа |
___
# Примеры кода
Используемая в примере строка аргументов
```bash
--tls -r "src/resources/" -p 443
```
Пример парсинга командной строки с заданными параметрами
```cpp
cli::OptionName use_tls_option('t', "tls");
cli::OptionName port_option('p', "port");
cli::OptionName resources_option('r', "resources");

cli::Options cli_options;
bool res = cli_options.parse(argc, argv, {
  {use_tls_option,   cli::OptionType::FLAG},
  {port_option,      cli::OptionType::ONE_ARGUMENT},
  {resources_option, cli::OptionType::MORE_ARGUMENT}
});
if (res == false)
{
  std::cout << "Command line reading error!" << std::endl;
  return -1;
}
```
Пример использования ключа-флага
```cpp
bool flag = cli_options.has(use_tls_option);
if (flag)
{
  //some code
}
```
Пример использования ключа с одним аргументом
```cpp
std::string* argument = cli_options.getArgument(port_option);
if (argument != nullptr)
{
  //some code
}
```
Пример использования ключа с множеством аргументов
```cpp
std::vector<std::string>* arguments = cli_options.getArguments(resources_option);
if (arguments != nullptr)
{
  //some code
}
```
Стоит отметить, что, несмотря на тот факт, что методы getArgument и getArguments возвращают указатели на std::string и std::vector, удалять объекты после использования не следует, так как библиотека сама управляет временем жизни.
# Лицензия
Проект распространяется под лицензией MIT. [LICENSE](LICENSE)
