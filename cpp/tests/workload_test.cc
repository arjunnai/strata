#include "strata/workload.h"

#include <gtest/gtest.h>

#include <stdexcept>

#include "strata/resource_vector.h"

TEST(WorkloadTest, StoresConstructorValues) {
  strata::Workload w1("job-1", "team-a", strata::ResourceVector(4, 8, 1));
  EXPECT_EQ(w1.id(), "job-1");
  EXPECT_EQ(w1.tenant_id(), "team-a");
  EXPECT_EQ(w1.request().cpu(), 4);
  EXPECT_EQ(w1.request().memory(), 8);
  EXPECT_EQ(w1.request().gpu(), 1);
}