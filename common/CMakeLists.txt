add_library(common INTERFACE
  common/IOMode.h
)

target_compile_features(common INTERFACE cxx_std_20)
target_include_directories(common INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}")
