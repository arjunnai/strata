#include "strata/control_plane.h"

#include <gtest/gtest.h>

#include "strata/resource_vector.h"
#include "strata/tenant.h"
#include "strata/workload.h"

TEST(ControlPlaneTest, StoresConstructorValues) {
  std::vector<strata::Tenant> tenantList;
  tenantList.emplace_back("tenant-a", strata::ResourceVector(4, 8, 1));
  tenantList.emplace_back("tenant-b", strata::ResourceVector(2, 4, 2));
  std::vector<strata::Node> nodeList;
  nodeList.emplace_back("small", strata::ResourceVector(2, 4, 1));
  nodeList.emplace_back("large", strata::ResourceVector(8, 16, 2));

  strata::Scheduler scheduler(nodeList);
  strata::ControlPlane cp(tenantList, scheduler);
  EXPECT_EQ(cp.tenants().size(), 2);
  EXPECT_EQ(cp.tenants()[0].id(), "tenant-a");
  EXPECT_EQ(cp.tenants()[1].id(), "tenant-b");

  strata::Tenant* tenant = cp.findTenant("tenant-b");
  ASSERT_NE(tenant, nullptr);
  EXPECT_EQ(tenant->id(), "tenant-b");
  EXPECT_EQ(cp.findTenant("tenant-z"), nullptr);
}

TEST(ControlPlaneTest, SubmitsAdmissibleWorkload) {
  std::vector<strata::Tenant> tenantList;
  tenantList.emplace_back("tenant-a", strata::ResourceVector(4, 8, 1));
  std::vector<strata::Node> nodeList;
  nodeList.emplace_back("node-a", strata::ResourceVector(8, 16, 2));

  strata::Scheduler scheduler(nodeList);
  strata::ControlPlane cp(tenantList, scheduler);
  strata::Workload workload("job-1", "tenant-a",
                            strata::ResourceVector(2, 4, 1));

  EXPECT_NE(cp.submit(workload), nullptr);
  ASSERT_NE(cp.findTenant("tenant-a"), nullptr);
  const auto& usage = cp.findTenant("tenant-a")->usage();
  EXPECT_EQ(usage.cpu(), 2);
  EXPECT_EQ(usage.memory(), 4);
  EXPECT_EQ(usage.gpu(), 1);
}

TEST(ControlPlaneTest, RejectsUnknownOrOverQuotaWorkload) {
  std::vector<strata::Tenant> tenantList;
  tenantList.emplace_back("tenant-a", strata::ResourceVector(4, 8, 1));
  std::vector<strata::Node> nodeList;
  nodeList.emplace_back("node-a", strata::ResourceVector(8, 16, 2));

  strata::Scheduler scheduler(nodeList);
  strata::ControlPlane cp(tenantList, scheduler);

  strata::Workload unknown("job-unknown", "tenant-z",
                           strata::ResourceVector(1, 1, 1));
  EXPECT_EQ(cp.submit(unknown), nullptr);

  strata::Workload first("job-1", "tenant-a",
                         strata::ResourceVector(3, 4, 1));
  strata::Workload overQuota("job-2", "tenant-a",
                             strata::ResourceVector(2, 4, 1));
  EXPECT_NE(cp.submit(first), nullptr);
  ASSERT_NE(cp.findTenant("tenant-a"), nullptr);
  const auto& usage = cp.findTenant("tenant-a")->usage();
  EXPECT_EQ(usage.cpu(), 3);
  EXPECT_EQ(usage.memory(), 4);
  EXPECT_EQ(usage.gpu(), 1);
  EXPECT_EQ(cp.submit(overQuota), nullptr);
}

TEST(ControlPlaneTest, DoesNotIncreaseUsageWhenSchedulingFails) {
  std::vector<strata::Tenant> tenantList;
  tenantList.emplace_back("tenant-a", strata::ResourceVector(8, 8, 2));
  std::vector<strata::Node> nodeList;
  nodeList.emplace_back("node-a", strata::ResourceVector(2, 2, 1));

  strata::Scheduler scheduler(nodeList);
  strata::ControlPlane cp(tenantList, scheduler);
  strata::Workload workload("job-1", "tenant-a",
                            strata::ResourceVector(4, 4, 1));

  ASSERT_NE(cp.findTenant("tenant-a"), nullptr);
  const auto& usage = cp.findTenant("tenant-a")->usage();
  EXPECT_EQ(usage.cpu(), 0);
  EXPECT_EQ(usage.memory(), 0);
  EXPECT_EQ(usage.gpu(), 0);
  EXPECT_EQ(cp.submit(workload), nullptr);
  EXPECT_EQ(usage.cpu(), 0);
  EXPECT_EQ(usage.memory(), 0);
  EXPECT_EQ(usage.gpu(), 0);
}

