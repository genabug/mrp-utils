add_library(lib2_static STATIC)
target_link_libraries(lib2_static PRIVATE factory lib1_static)
target_include_directories(lib2_static PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

target_link_options(lib2_static
INTERFACE
  "LINKER:${NO_AS_NEEDED}$<TARGET_FILE:lib2_static>${AS_NEEDED}")

target_sources(lib2_static
PUBLIC
  lib2/base2.h
  lib2/derived21.h
  lib2/derived22.h
PRIVATE
  lib2/derived21.cpp
  lib2/derived22.cpp
)
