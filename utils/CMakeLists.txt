add_library(utils INTERFACE)
target_compile_features(utils INTERFACE cxx_std_14)
target_include_directories(utils INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}")
