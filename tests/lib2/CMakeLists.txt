add_library(lib2)
target_link_libraries(lib2 PRIVATE lib1 PUBLIC factory)
target_sources(lib2
PUBLIC
  base2.hpp
  derived21.hpp
  derived22.hpp
PRIVATE
  derived21.cpp
  derived22.cpp
PUBLIC
  register2.cpp
)
#target_link_options(lib2
#INTERFACE
#  "LINKER:${NO_AS_NEEDED}$<TARGET_FILE:lib2>${AS_NEEDED}"
#)
include(GenerateExportHeader)
generate_export_header(lib2)
target_include_directories(lib2 PUBLIC "${CMAKE_CURRENT_BINARY_DIR}")
