set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# A dirty hack to force the linker to keep unused variables.
set(AS_NEEDED ",--as-needed,--no-whole-archive,")
set(NO_AS_NEEDED ",--no-as-needed,--whole-archive,")
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
  set(AS_NEEDED "")
  set(NO_AS_NEEDED "/WHOLEARCHIVE:")
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "AppleClang")
  set(AS_NEEDED "")
  set(NO_AS_NEEDED ",-force_load,")
endif()

