# add_library(lib "")
# add_dependencies(lib factory)
# target_link_libraries(lib PRIVATE factory)
# target_sources(lib PRIVATE ...)

add_executable(tst_factory tst_factory.cpp)
add_dependencies(tst_factory doctest factory)
target_link_libraries(tst_factory PRIVATE doctest factory)
add_test(tst_factory tst_factory)
