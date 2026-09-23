add_executable(
  strata_tests
  resource_vector_test.cc
  workload_test.cc
  node_test.cc
  scheduler_test.cc
  tenant_test.cc
  control_plane_test.cc
)

target_link_libraries(
  strata_tests
  PRIVATE
    strata_core
    GTest::gtest_main
)

target_compile_options(
  strata_tests
  PRIVATE
    -Wall
    -Wextra
    -Wpedantic
)

include(GoogleTest)
gtest_discover_tests(strata_tests)
