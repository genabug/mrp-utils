add_library(lib SHARED)
add_dependencies(lib factory)
target_link_libraries(lib PRIVATE factory)
target_sources(lib
  PUBLIC
    base.hpp base.cpp
    derived1.hpp derived1.cpp
    derived2.hpp derived2.cpp)

set_target_properties(lib PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS ON)
#include(GenerateExportHeader)
#generate_export_header(lib)
