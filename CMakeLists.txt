cmake_minimum_required(VERSION 3.13)

project(mrp-utils LANGUAGES CXX)

set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_VERBOSE_MAKEFILE ON)

# gcc/clang assumed
#set(AS_NEEDED ",--as-needed,--no-whole-archive,")
#set(NO_AS_NEEDED ",--no-as-needed,--whole-archive,")
# msvc assumed
#if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
#  set(AS_NEEDED "")
#  set(NO_AS_NEEDED "/WHOLEARCHIVE:")
#endif()

add_subdirectory(utils)
add_subdirectory(factory)
add_subdirectory(tests)
