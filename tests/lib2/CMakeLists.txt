add_library(lib2 SHARED)
target_link_libraries(lib2 PRIVATE factory lib1)
target_include_directories(lib2 PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

target_sources(lib2
PUBLIC
  lib2/base2.hpp
  lib2/derived21.hpp
  lib2/derived22.hpp
PRIVATE
  lib2/base2.cpp
  lib2/derived21.cpp
  lib2/derived22.cpp
)

include(GenerateExportHeader)
generate_export_header(lib2)
target_include_directories(lib2 PUBLIC "${CMAKE_CURRENT_BINARY_DIR}")
