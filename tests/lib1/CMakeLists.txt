add_library(lib1)
target_link_libraries(lib1 PRIVATE factory)
target_include_directories(lib1 PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

target_sources(lib1
PUBLIC
  base1.hpp
  derived11.hpp
  derived12.hpp
PRIVATE
  base1.cpp
  derived11.cpp
  derived12.cpp
)

include(GenerateExportHeader)
generate_export_header(lib1)
target_include_directories(lib1 PUBLIC "${CMAKE_CURRENT_BINARY_DIR}")
