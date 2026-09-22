add_library(
  strata_core
  src/resource_vector.cc
  src/workload.cc
)

target_include_directories(
  strata_core
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_options(
  strata_core
  PRIVATE
    -Wall
    -Wextra
    -Wpedantic
)

add_subdirectory(tests)
