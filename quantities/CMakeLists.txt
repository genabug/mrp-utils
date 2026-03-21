add_library(quantities INTERFACE
  quantities/details.h
  quantities/State.h
  quantities/Traits.h
)

target_compile_features(quantities INTERFACE cxx_std_20)
target_include_directories(quantities INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}")
target_link_libraries(quantities INTERFACE math)
