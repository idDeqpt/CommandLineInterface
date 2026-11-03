cmake_minimum_required(VERSION 3.14)

set(PROJECT_NAME test)

project(${PROJECT_NAME} CXX)

add_executable(${PROJECT_NAME} test.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE CLI::CLI)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)