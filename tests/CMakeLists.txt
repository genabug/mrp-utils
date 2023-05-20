find_package(GTest QUIET)
if(NOT GTest_FOUND)
  set(GOOGLETEST_VERSION 1.13.0)
  message(STATUS "gtest not found, use project's copy '${GOOGLETEST_VERSION}'")
  set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
  add_subdirectory(googletest)
endif()

include(CTest)
include(GoogleTest)

function(add_marple_test tst_name)
  cmake_parse_arguments(tst "" "" "SOURCES;DEPENDS" ${ARGN})

  foreach(arg IN LISTS ${tst_UNPARSED_ARGUMENTS})
    message(WARNING "add_marple_test had unparsed argument: '${arg}'")
  endforeach()
  if (${tst_UNPARSED_ARGUMENTS})
    message(FATAL_ERROR "add_marple_test had unparsed arguments")
  endif()

  add_executable(${tst_name} ${tst_SOURCES})
  target_link_libraries(${tst_name} PRIVATE GTest::gtest_main ${tst_DEPENDS})

  gtest_discover_tests(${tst_name})
endfunction()

add_marple_test(tst_state SOURCES tst_state.cpp DEPENDS utils)
add_marple_test(tst_vector SOURCES tst_vector.cpp DEPENDS utils)
add_marple_test(tst_tensor SOURCES tst_tensor.cpp DEPENDS utils)

add_subdirectory(lib1)
add_subdirectory(lib2)

# put lib1 and lib2 together with executables to make Windows happy
set_target_properties(lib1
PROPERTIES
  LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

set_target_properties(lib2
PROPERTIES
  LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

add_marple_test(tst_lib2 SOURCES tst_lib2.cpp DEPENDS lib2 factory)
add_marple_test(tst_factory SOURCES tst_factory.cpp DEPENDS lib1 lib2 factory)
