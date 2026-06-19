if(NOT TARGET CLI::CLI)
    add_library(CLI::CLI UNKNOWN IMPORTED)

    set(LIBRARY_ROOT "${CMAKE_CURRENT_LIST_DIR}/..")

    #Включение заголовочных файлов
    set_target_properties(CLI::CLI PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_LIST_DIR}/../include")
    
    #Установка целевого файла библиотеки
    set_target_properties(CLI::CLI PROPERTIES
        IMPORTED_LOCATION_DEBUG   "${LIBRARY_ROOT}/build/Debug/CLI.lib"
        IMPORTED_LOCATION_RELEASE "${LIBRARY_ROOT}/build/Release/CLI.lib"
        IMPORTED_LOCATION_RELWITHDEBINFO "${LIBRARY_ROOT}/build/Release/CLI.lib"
        IMPORTED_LOCATION_MINSIZEREL     "${LIBRARY_ROOT}/build/Release/CLI.lib"
        MAP_IMPORTED_CONFIG_RELWITHDEBINFO "RELEASE"
        MAP_IMPORTED_CONFIG_MINSIZEREL     "RELEASE")
endif()