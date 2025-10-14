find_package(GTest REQUIRED)

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
  target_link_libraries(${tst_name} PRIVATE GTest::gmock GTest::gtest_main ${tst_DEPENDS})

  gtest_discover_tests(${tst_name})
endfunction()

add_marple_test(tst_state SOURCES tst_state.cpp DEPENDS utils)
add_marple_test(tst_vector SOURCES tst_vector.cpp DEPENDS utils)
add_marple_test(tst_tensor SOURCES tst_tensor.cpp DEPENDS utils)

add_subdirectory(lib1)
add_subdirectory(lib2)

# put lib1 and lib2 together with test executables to make Windows happy
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

add_subdirectory(lib1_static)
add_subdirectory(lib2_static)

add_marple_test(tst_lib2_static SOURCES tst_lib2_static.cpp DEPENDS lib2_static factory)
add_marple_test(tst_factory_static SOURCES tst_factory_static.cpp DEPENDS lib1_static lib2_static factory)
