cmake_minimum_required(VERSION 3.27)

set(PROJECT_NAME CLI)
set(BUILD_SHARED_LIBS FALSE)

project(${PROJECT_NAME}
    VERSION 1.0.0
    LANGUAGES CXX)

set(SOURCE_FILES
    src/CommandLineOptions.cpp)

add_library(${PROJECT_NAME})
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})
target_sources(${PROJECT_NAME} PRIVATE ${SOURCE_FILES})

target_include_directories(${PROJECT_NAME}
    PUBLIC 
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)


include(GNUInstallDirs)

#Установка целевого продукта
install(TARGETS ${PROJECT_NAME}
    EXPORT ${PROJECT_NAME}Targets
    LIBRARY DESTINATION bin/lib              #Библиотеки будут устанавливаться сюда
    ARCHIVE DESTINATION bin/lib              #То же самое для архивированных версий (.lib)
    RUNTIME DESTINATION bin                  #Исполняемые файлы (*.exe)
    PUBLIC_HEADER DESTINATION include)       #Заголовочные файлы

#Экспорт целевой спецификации
install(EXPORT ${PROJECT_NAME}Targets
    NAMESPACE CLI::          #Пространство имён для целей экспорта
    FILE ${PROJECT_NAME}Config.cmake
    DESTINATION lib/cmake/${PROJECT_NAME})

export(EXPORT ${PROJECT_NAME}Targets
    NAMESPACE CLI::
    FILE ${PROJECT_NAME}Config.cmake)

#Генерация файла конфигурации с версией
configure_file(
    "cmake/${PROJECT_NAME}ConfigVersion.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
    @ONLY)

#Установка файла конфигурации
install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
    DESTINATION lib/cmake/${PROJECT_NAME})