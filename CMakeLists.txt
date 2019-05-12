cmake_minimum_required(VERSION 3.1)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(mrp-utils LANGUAGES CXX)
enable_testing()
add_definitions("-DDOCTEST_CONFIG_IMPLEMENT_WITH_MAIN=1")

set(tst_factory_src
TESTS/doctest.h
TESTS/tst_factory.cpp
ObjectsFactory.h)

set(tst_state_src
QDetails.h
QState.h
QState.cpp
QTraits.h
TESTS/doctest.h
TESTS/tst_state.cpp)

set(tst_vector_src
Vector.h
TESTS/doctest.h
TESTS/tst_vector.cpp)

set(tst_tensor_src
Tensor.h
TESTS/doctest.h
TESTS/tst_tensor.cpp)

add_executable(tst_state ${tst_state_src})
add_executable(tst_vector ${tst_vector_src})
add_executable(tst_tensor ${tst_tensor_src})
###add_executable(tst_factory ${tst_factory_stc})
###add_test(tst_factory tst_factory)
add_test(tst_state tst_state)
add_test(tst_vector tst_vector)
add_test(tst_tensor tst_tensor)

include_directories(BEFORE .)

