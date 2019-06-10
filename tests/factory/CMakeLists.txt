add_subdirectory(lib)

add_executable(tst_factory tst_factory.cpp)
add_dependencies(tst_factory doctest factory lib)
target_link_libraries(tst_factory PRIVATE doctest factory lib)
add_test(tst_factory tst_factory)
