#include "strata/node.h"

#include <gtest/gtest.h>

#include "strata/resource_vector.h"

TEST(NodeTest, StoresConstructorValues) {
  strata::Node n1("node-1", strata::ResourceVector(8, 32, 2));
  EXPECT_EQ(n1.id(), "node-1");
  EXPECT_EQ(n1.availableCap().cpu(), 8);
  EXPECT_EQ(n1.totalCap().cpu(), 8);
  EXPECT_EQ(n1.availableCap().memory(), 32);
  EXPECT_EQ(n1.totalCap().memory(), 32);
  EXPECT_EQ(n1.availableCap().gpu(), 2);
  EXPECT_EQ(n1.totalCap().gpu(), 2);
}

TEST(NodeTest, FitInside) {
  strata::Node n1("node-1", strata::ResourceVector(8, 32, 2));
  strata::ResourceVector req(4, 16, 1);
  EXPECT_TRUE(n1.canFit(req));
}
TEST(NodeTest, NotFitInside) {
  strata::Node n1("node-1", strata::ResourceVector(8, 32, 2));
  strata::ResourceVector req(9, 16, 1);
  EXPECT_FALSE(n1.canFit(req));
}

TEST(NodeTest, SuccessfulAllocationReducesAvailableCapacity) {
  strata::Node n1("node-1", strata::ResourceVector(8, 32, 2));
  strata::ResourceVector req(4, 16, 1);

  EXPECT_TRUE(n1.allocate(req));
  EXPECT_EQ(n1.availableCap().cpu(), 4);
  EXPECT_EQ(n1.availableCap().memory(), 16);
  EXPECT_EQ(n1.availableCap().gpu(), 1);
  EXPECT_EQ(n1.totalCap().cpu(), 8);
  EXPECT_EQ(n1.totalCap().memory(), 32);
  EXPECT_EQ(n1.totalCap().gpu(), 2);
}

TEST(NodeTest, FailedAllocationLeavesAvailableCapacityUnchanged) {
  strata::Node n1("node-1", strata::ResourceVector(8, 32, 2));
  strata::ResourceVector req(9, 16, 1);

  EXPECT_FALSE(n1.allocate(req));
  EXPECT_EQ(n1.availableCap().cpu(), 8);
  EXPECT_EQ(n1.availableCap().memory(), 32);
  EXPECT_EQ(n1.availableCap().gpu(), 2);
}
