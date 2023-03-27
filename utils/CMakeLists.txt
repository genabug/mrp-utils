add_library(utils INTERFACE
  utils/IOMode.h
  utils/QDetails.h
  utils/QState.h
  utils/QTraits.h
  utils/Tensor.h
  utils/Utils.h
  utils/Vector.h)

target_compile_features(utils INTERFACE cxx_std_17)
target_include_directories(utils INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}")
