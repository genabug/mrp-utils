enable_testing()
add_definitions("-DDOCTEST_CONFIG_IMPLEMENT_WITH_MAIN=1")

add_library(doctest INTERFACE)
target_include_directories(doctest INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(tst_state tst_state.cpp)
add_dependencies(tst_state doctest utils)
target_link_libraries(tst_state PRIVATE doctest utils)
add_test(tst_state tst_state)

add_executable(tst_vector tst_vector.cpp)
add_dependencies(tst_vector doctest utils)
target_link_libraries(tst_vector PRIVATE doctest utils)
add_test(tst_vector tst_vector)

add_executable(tst_tensor tst_tensor.cpp)
add_dependencies(tst_tensor doctest utils)
target_link_libraries(tst_tensor PRIVATE doctest utils)
add_test(tst_tensor tst_tensor)

add_subdirectory(factory)
