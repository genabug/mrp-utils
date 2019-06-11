cmake_minimum_required(VERSION 3.1)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(mrp-utils LANGUAGES CXX)

include(${PROJECT_SOURCE_DIR}/cmake/add_marple_test.cmake)

add_subdirectory(utils)
add_subdirectory(factory)
add_subdirectory(tests)
