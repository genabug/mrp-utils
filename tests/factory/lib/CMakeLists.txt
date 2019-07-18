add_library(lib SHARED "")
add_dependencies(lib factory)
target_link_libraries(lib PRIVATE factory)
target_sources(lib PRIVATE
    base.cpp   base.hpp
    derived1.cpp   derived1.hpp
    derived2.cpp   derived2.hpp)

set_target_properties(lib PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS ON)
#include(GenerateExportHeader)
#generate_export_header(lib)
#target_include_directories(lib PUBLIC ${CMAKE_CURRENT_BINARY_DIR})
