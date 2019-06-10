cmake_minimum_required(VERSION 3.1)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(mrp-utils LANGUAGES CXX)

include(${PROJECT_SOURCE_DIR}/cmake/add_marple_test.cmake)

add_library(utils INTERFACE)
target_include_directories(utils INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(factory)
add_subdirectory(tests)
