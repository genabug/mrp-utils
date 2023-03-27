add_library(lib2)
target_link_libraries(lib2 PRIVATE factory lib1)
target_include_directories(lib2 PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

target_sources(lib2
PUBLIC
  base2.hpp
  derived21.hpp
  derived22.hpp
PRIVATE
  base2.cpp
  derived21.cpp
  derived22.cpp
)

include(GenerateExportHeader)
generate_export_header(lib2)
target_include_directories(lib2 PUBLIC "${CMAKE_CURRENT_BINARY_DIR}")
