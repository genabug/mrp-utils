add_library(lib OBJECT)
add_dependencies(lib factory)
target_link_libraries(lib PRIVATE factory)
target_sources(lib
  PUBLIC
    base.hpp
    derived1.hpp
    derived2.hpp
  PRIVATE
    derived1.cpp
    derived2.cpp
)

#set_target_properties(lib PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS ON)
#include(GenerateExportHeader)
#generate_export_header(lib)
