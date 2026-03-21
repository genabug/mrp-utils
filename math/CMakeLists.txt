add_library(math INTERFACE
  math/Mode.h
  math/Type.h
  math/Tensor.h
  math/details.h
  math/Vector.h
)

target_compile_features(math INTERFACE cxx_std_20)
target_include_directories(math INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}")
