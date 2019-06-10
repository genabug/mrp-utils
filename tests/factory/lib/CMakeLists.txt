add_library(lib SHARED "")
add_dependencies(lib factory)
target_link_libraries(lib PRIVATE factory)
target_sources(lib PRIVATE
    base.cpp   base.hpp
    derived1.cpp   derived1.hpp
    derived2.cpp   derived2.hpp)
