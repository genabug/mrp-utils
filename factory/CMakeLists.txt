add_library(factory INTERFACE)
target_compile_features(factory INTERFACE cxx_std_14)
target_sources(factory INTERFACE factory/ObjectsFactory.h) # should work in cmake 3.19
target_include_directories(factory INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}")
