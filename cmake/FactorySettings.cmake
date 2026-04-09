set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# A dirty hack to force the linker to keep unused variables.
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
  set(AS_NEEDED "")
  set(NO_AS_NEEDED "/WHOLEARCHIVE:")
elseif(APPLE AND "${CMAKE_CXX_COMPILER_ID}" MATCHES ".*Clang")
  set(AS_NEEDED "")
  set(NO_AS_NEEDED ",-force_load,")
elseif("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU|.*Clang")
  set(AS_NEEDED ",--as-needed,--no-whole-archive,")
  set(NO_AS_NEEDED ",--no-as-needed,--whole-archive,")
else()
  #TODO: error
endif()
