add_library(utils INTERFACE)
target_include_directories(utils INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}")

target_sources(utils INTERFACE
"${CMAKE_CURRENT_SOURCE_DIR}/utils/QDetails.h"
"${CMAKE_CURRENT_SOURCE_DIR}/utils/QState.h"
"${CMAKE_CURRENT_SOURCE_DIR}/utils/QTraits.h"
"${CMAKE_CURRENT_SOURCE_DIR}/utils/Tensor.h"
"${CMAKE_CURRENT_SOURCE_DIR}/utils/Utils.h"
"${CMAKE_CURRENT_SOURCE_DIR}/utils/Vector.h")
