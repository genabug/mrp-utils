add_library(doctest INTERFACE)
target_sources(doctest INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/doctest.h")
target_include_directories(doctest INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

function(add_marple_test tst_name)
  cmake_parse_arguments(tst "" "" "SOURCES;DEPENDS" ${ARGN})

  foreach(arg IN LISTS ${tst_UNPARSED_ARGUMENTS})
    message(WARNING "add_marple_test had unparsed argument: '${arg}'")
  endforeach()
  if (${tst_UNPARSED_ARGUMENTS})
    message(FATAL_ERROR "add_marple_test had unparsed arguments")
  endif()

  add_executable(${tst_name} ${tst_SOURCES})
  add_dependencies(${tst_name} doctest ${tst_DEPENDS})
  target_link_libraries(${tst_name} PRIVATE doctest ${tst_DEPENDS})
  target_compile_definitions(${tst_name} PRIVATE "-DDOCTEST_CONFIG_IMPLEMENT_WITH_MAIN=1")

  add_test(NAME ${tst_name} COMMAND ${tst_name})
endfunction()

add_marple_test(tst_state SOURCES tst_state.cpp DEPENDS utils)
add_marple_test(tst_vector SOURCES tst_vector.cpp DEPENDS utils)
add_marple_test(tst_tensor SOURCES tst_tensor.cpp DEPENDS utils)

add_subdirectory(factory)
