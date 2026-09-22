#include "strata/resource_vector.h"

#include <gtest/gtest.h>

#include <stdexcept>

TEST(ResourceVectorTest, RejectsNegativeCpu) {
  EXPECT_THROW(strata::ResourceVector(-1, 0, 0), std::invalid_argument);
}

TEST(ResourceVectorTest, RejectsNegativeMem) {
  EXPECT_THROW(strata::ResourceVector(0, -1, 0), std::invalid_argument);
}

TEST(ResourceVectorTest, RejectsNegativeGPU) {
  EXPECT_THROW(strata::ResourceVector(0, 0, -1), std::invalid_argument);
}

TEST(ResourceVectorTest, AcceptsZeroResources) { EXPECT_NO_THROW(strata::ResourceVector(0, 0, 0)); }

TEST(ResourceVectorTest, StoresValues) {
  strata::ResourceVector resources(4, 16, 1);
  EXPECT_EQ(resources.cpu(), 4);
  EXPECT_EQ(resources.memory(), 16);
  EXPECT_EQ(resources.gpu(), 1);
}

TEST(ResourceVectorTest, FitsNormally) {
  strata::ResourceVector request(4, 16, 1);
  strata::ResourceVector capacity(8, 20, 5);
  EXPECT_TRUE(request.workloadFit(capacity));
}

TEST(ResourceVectorTest, FailsOneResource) {
  strata::ResourceVector request(4, 16, 1);
  strata::ResourceVector capacity(3, 20, 5);
  EXPECT_FALSE(request.workloadFit(capacity));
}

TEST(ResourceVectorTest, BoundaryCase) {
  strata::ResourceVector request(4, 16, 1);
  strata::ResourceVector capacity(4, 16, 1);
  EXPECT_TRUE(request.workloadFit(capacity));
}

TEST(ResourceVectorTest, NormalSubtraction) {
  strata::ResourceVector request(4, 16, 1);
  strata::ResourceVector capacity(8, 20, 5);
  strata::ResourceVector result = capacity - request;
  EXPECT_EQ(result.cpu(), 4);
  EXPECT_EQ(result.memory(), 4);
  EXPECT_EQ(result.gpu(), 4);
}

TEST(ResourceVectorTest, ExactZero) {
  strata::ResourceVector request(4, 16, 1);
  strata::ResourceVector capacity(4, 16, 1);
  strata::ResourceVector result = capacity - request;
  EXPECT_EQ(result.cpu(), 0);
  EXPECT_EQ(result.memory(), 0);
  EXPECT_EQ(result.gpu(), 0);
}

// TEST(ResourceVectorTest, Invalidsubtraction) {
//   strata::ResourceVector request(4, 16, 1);
//   strata::ResourceVector capacity(8, 20, 5);
//   EXPECT_TRUE(request.workloadFit(capacity));
//   strata::ResourceVector result = capacity - request;
//   EXPECT_EQ(result.cpu(), 4);
//   EXPECT_EQ(result.memory(), 4);
//   EXPECT_EQ(result.gpu(), 4);
// }