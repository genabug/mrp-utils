add_library(quantities INTERFACE
  quantities/State.h
  quantities/Traits.h
  quantities/details.h
)

target_link_libraries(quantities INTERFACE math common)
target_compile_features(quantities INTERFACE cxx_std_20)
target_include_directories(quantities INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}")
