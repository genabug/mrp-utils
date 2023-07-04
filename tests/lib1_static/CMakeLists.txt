add_library(lib1_static STATIC)
target_link_libraries(lib1_static PRIVATE factory)
target_include_directories(lib1_static PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

target_link_options(lib1_static
INTERFACE
  "LINKER:${NO_AS_NEEDED}$<TARGET_FILE:lib1_static>${AS_NEEDED}")

target_sources(lib1_static
PUBLIC
  lib1/base1.h
  lib1/derived11.h
  lib1/derived12.h
PRIVATE
  lib1/derived11.cpp
  lib1/derived12.cpp
)
