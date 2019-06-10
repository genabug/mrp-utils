enable_testing()
add_definitions("-DDOCTEST_CONFIG_IMPLEMENT_WITH_MAIN=1")

add_executable(tst_state tst_state.cpp)
target_link_libraries(tst_state PRIVATE utils)
add_test(tst_state tst_state)

add_executable(tst_vector tst_vector.cpp)
target_link_libraries(tst_vector PRIVATE utils)
add_test(tst_vector tst_vector)

add_executable(tst_tensor tst_tensor.cpp)
target_link_libraries(tst_tensor PRIVATE utils)
add_test(tst_tensor tst_tensor)

#add_executable(tst_factory tst_factory.cpp)
#add_dependencies(tst_factory factory)
#target_link_libraries(tst_factory PRIVATE factory)
#add_test(tst_factory tst_factory)
