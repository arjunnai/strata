#include "strata/tenant.h"

#include <gtest/gtest.h>

#include "strata/resource_vector.h"

TEST(TenantTest, StoresConstructorValues) {
  strata::Tenant t1("tenant-a", strata::ResourceVector(8, 16, 1));
  EXPECT_EQ(t1.id(), "tenant-a");
  EXPECT_EQ(t1.quota().cpu(), 8);
  EXPECT_EQ(t1.quota().memory(), 16);
  EXPECT_EQ(t1.quota().gpu(), 1);
  EXPECT_EQ(t1.usage().cpu(), 0);
  EXPECT_EQ(t1.usage().memory(), 0);
  EXPECT_EQ(t1.usage().gpu(), 0);
}

TEST(TenantTest, CanAdmitRequestWithinQuota) {
  strata::Tenant tenant("tenant-a", strata::ResourceVector(8, 16, 1));

  EXPECT_TRUE(tenant.canAdmit(strata::ResourceVector(8, 16, 1)));
}

TEST(TenantTest, RejectsRequestExceedingQuota) {
  strata::Tenant tenant("tenant-a", strata::ResourceVector(8, 16, 1));

  EXPECT_FALSE(tenant.canAdmit(strata::ResourceVector(9, 16, 1)));
  EXPECT_FALSE(tenant.canAdmit(strata::ResourceVector(8, 17, 1)));
  EXPECT_FALSE(tenant.canAdmit(strata::ResourceVector(8, 16, 2)));
}

TEST(TenantTest, CanAdmitDoesNotChangeUsage) {
  strata::Tenant tenant("tenant-a", strata::ResourceVector(8, 16, 1));

  EXPECT_TRUE(tenant.canAdmit(strata::ResourceVector(2, 4, 1)));
  EXPECT_EQ(tenant.usage().cpu(), 0);
  EXPECT_EQ(tenant.usage().memory(), 0);
  EXPECT_EQ(tenant.usage().gpu(), 0);
}

TEST(TenantTest, SuccessfulAdmitUpdatesUsage) {
  strata::Tenant tenant("tenant-a", strata::ResourceVector(8, 16, 1));

  EXPECT_TRUE(tenant.admit(strata::ResourceVector(2, 4, 1)));
  EXPECT_EQ(tenant.usage().cpu(), 2);
  EXPECT_EQ(tenant.usage().memory(), 4);
  EXPECT_EQ(tenant.usage().gpu(), 1);
}

TEST(TenantTest, FailedAdmitLeavesUsageUnchanged) {
  strata::Tenant tenant("tenant-a", strata::ResourceVector(8, 16, 1));
  ASSERT_TRUE(tenant.admit(strata::ResourceVector(2, 4, 1)));

  EXPECT_FALSE(tenant.admit(strata::ResourceVector(7, 12, 1)));
  EXPECT_EQ(tenant.usage().cpu(), 2);
  EXPECT_EQ(tenant.usage().memory(), 4);
  EXPECT_EQ(tenant.usage().gpu(), 1);
}

TEST(TenantTest, SuccessfulReleaseReducesUsage) {
  strata::Tenant tenant("tenant-a", strata::ResourceVector(8, 16, 1));
  ASSERT_TRUE(tenant.admit(strata::ResourceVector(6, 12, 1)));

  EXPECT_TRUE(tenant.release(strata::ResourceVector(2, 4, 1)));
  EXPECT_EQ(tenant.usage().cpu(), 4);
  EXPECT_EQ(tenant.usage().memory(), 8);
  EXPECT_EQ(tenant.usage().gpu(), 0);
}

TEST(TenantTest, InvalidReleaseLeavesUsageUnchanged) {
  strata::Tenant tenant("tenant-a", strata::ResourceVector(8, 16, 1));
  ASSERT_TRUE(tenant.admit(strata::ResourceVector(2, 4, 1)));

  EXPECT_FALSE(tenant.release(strata::ResourceVector(3, 4, 1)));
  EXPECT_EQ(tenant.usage().cpu(), 2);
  EXPECT_EQ(tenant.usage().memory(), 4);
  EXPECT_EQ(tenant.usage().gpu(), 1);
}


