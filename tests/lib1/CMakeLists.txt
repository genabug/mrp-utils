add_library(lib1 SHARED)
target_link_libraries(lib1 PRIVATE factory)
target_include_directories(lib1 PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

target_sources(lib1
PUBLIC
  lib1/base1.hpp
  lib1/derived11.hpp
  lib1/derived12.hpp
PRIVATE
  lib1/base1.cpp
  lib1/derived11.cpp
  lib1/derived12.cpp
)

include(GenerateExportHeader)
generate_export_header(lib1)
target_include_directories(lib1 PUBLIC "${CMAKE_CURRENT_BINARY_DIR}")
