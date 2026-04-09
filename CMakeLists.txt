cmake_minimum_required(VERSION 3.23)

project(numkit VERSION 0.33 LANGUAGES CXX)

option(NUMKIT_BUILD_TESTS "Build tests" OFF)
option(NUMKIT_BUILD_DOCS "Build documentation" OFF)

include(cmake/FactorySettings.cmake)

add_subdirectory(math)
add_subdirectory(common)
add_subdirectory(factory)
add_subdirectory(quantities)

if(NUMKIT_BUILD_TESTS)
  enable_testing()
  add_subdirectory(tests)
endif()

if(NUMKIT_BUILD_DOCS)
  add_subdirectory(docs)
endif()

include_directories(math common factory quantities) # to make clangd happy