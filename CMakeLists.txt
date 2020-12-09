cmake_minimum_required(VERSION 3.13)

project(mrp-utils LANGUAGES CXX)

add_subdirectory(utils)
add_subdirectory(factory)

enable_testing()
add_subdirectory(tests)
