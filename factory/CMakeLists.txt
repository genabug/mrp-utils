add_library(factory INTERFACE)
target_sources(factory INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/factory/ObjectsFactory.h")
target_include_directories(factory INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
