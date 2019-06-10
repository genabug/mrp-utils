enable_testing()
add_definitions("-DDOCTEST_CONFIG_IMPLEMENT_WITH_MAIN=1")

add_library(doctest INTERFACE)
target_include_directories(doctest INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

add_marple_test(tst_state SOURCES tst_state.cpp DEPENDS utils)
add_marple_test(tst_vector SOURCES tst_vector.cpp DEPENDS utils)
add_marple_test(tst_tensor SOURCES tst_tensor.cpp DEPENDS utils)

add_subdirectory(factory)
