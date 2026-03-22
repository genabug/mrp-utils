find_package(Doxygen REQUIRED)

set(DOXYGEN_PROJECT_NAME "numkit")
set(DOXYGEN_PROJECT_BRIEF "Low-level mathematical data structures for numerical computations")
set(DOXYGEN_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}")
set(DOXYGEN_GENERATE_HTML YES)
set(DOXYGEN_GENERATE_LATEX NO)
set(DOXYGEN_EXTRACT_ALL YES)
set(DOXYGEN_RECURSIVE YES)

doxygen_add_docs(doc
  "${PROJECT_SOURCE_DIR}/common"
  "${PROJECT_SOURCE_DIR}/math"
  "${PROJECT_SOURCE_DIR}/quantities"
  "${PROJECT_SOURCE_DIR}/factory"
  COMMENT "Generating API documentation"
)

