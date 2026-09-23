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

