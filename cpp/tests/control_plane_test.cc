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

TEST(ControlPlaneTest, enqueueWorkload){
  std::vector<strata::Tenant> tenantList;
  tenantList.emplace_back("tenant-a", strata::ResourceVector(4, 8, 1));
  std::vector<strata::Node> nodeList;
  nodeList.emplace_back("node-a", strata::ResourceVector(8, 16, 2));

  strata::Scheduler scheduler(nodeList);
  strata::ControlPlane cp(tenantList, scheduler);
  strata::Workload workload1("job-1", "tenant-a",
                             strata::ResourceVector(1, 2, 0));
  strata::Workload workload2("job-2", "tenant-a",
                             strata::ResourceVector(2, 4, 1));

  cp.enqueue(workload1);
  cp.enqueue(workload2);

  const auto& queue = cp.pending().at("tenant-a");

  EXPECT_EQ(queue.size(), 2);
  EXPECT_EQ(queue.front().id(), "job-1");
  EXPECT_EQ(queue.back().id(), "job-2");
}

TEST(ControlPlaneTest, KeepsTenantQueuesSeparate) {
  std::vector<strata::Tenant> tenantList;
  tenantList.emplace_back("tenant-a", strata::ResourceVector(4, 8, 1));
  tenantList.emplace_back("tenant-b", strata::ResourceVector(4, 8, 1));
  std::vector<strata::Node> nodeList;
  nodeList.emplace_back("node-a", strata::ResourceVector(8, 16, 2));

  strata::Scheduler scheduler(nodeList);
  strata::ControlPlane cp(tenantList, scheduler);

  EXPECT_TRUE(cp.enqueue(strata::Workload(
      "A1", "tenant-a", strata::ResourceVector(1, 1, 0))));
  EXPECT_TRUE(cp.enqueue(strata::Workload(
      "A2", "tenant-a", strata::ResourceVector(1, 1, 0))));
  EXPECT_TRUE(cp.enqueue(strata::Workload(
      "B1", "tenant-b", strata::ResourceVector(1, 1, 0))));

  const auto& tenantAQueue = cp.pending().at("tenant-a");
  const auto& tenantBQueue = cp.pending().at("tenant-b");
  ASSERT_EQ(tenantAQueue.size(), 2);
  ASSERT_EQ(tenantBQueue.size(), 1);
  EXPECT_EQ(tenantAQueue.front().id(), "A1");
  EXPECT_EQ(tenantAQueue.back().id(), "A2");
  EXPECT_EQ(tenantBQueue.front().id(), "B1");
}

TEST(ControlPlaneTest, RejectsUnknownTenantEnqueue) {
  std::vector<strata::Tenant> tenantList;
  tenantList.emplace_back("tenant-a", strata::ResourceVector(4, 8, 1));
  std::vector<strata::Node> nodeList;
  nodeList.emplace_back("node-a", strata::ResourceVector(8, 16, 2));

  strata::Scheduler scheduler(nodeList);
  strata::ControlPlane cp(tenantList, scheduler);

  EXPECT_FALSE(cp.enqueue(strata::Workload(
      "job-unknown", "tenant-z", strata::ResourceVector(1, 1, 0))));
}

  TEST(ControlPlaneTest, DequeuesWorkloadsInRoundRobinOrder) {
    std::vector<strata::Tenant> tenantList;
    tenantList.emplace_back("tenant-a", strata::ResourceVector(4, 8, 1));
    tenantList.emplace_back("tenant-b", strata::ResourceVector(4, 8, 1));
    tenantList.emplace_back("tenant-c", strata::ResourceVector(4, 8, 1));
    std::vector<strata::Node> nodeList;
    nodeList.emplace_back("node-a", strata::ResourceVector(8, 16, 2));

    strata::Scheduler scheduler(nodeList);
    strata::ControlPlane cp(tenantList, scheduler);

    EXPECT_TRUE(cp.enqueue(strata::Workload(
      "A1", "tenant-a", strata::ResourceVector(1, 1, 0))));
    EXPECT_TRUE(cp.enqueue(strata::Workload(
      "A2", "tenant-a", strata::ResourceVector(1, 1, 0))));
    EXPECT_TRUE(cp.enqueue(strata::Workload(
      "B1", "tenant-b", strata::ResourceVector(1, 1, 0))));
    EXPECT_TRUE(cp.enqueue(strata::Workload(
      "C1", "tenant-c", strata::ResourceVector(1, 1, 0))));

    auto first = cp.dequeueNextWorkload();
    ASSERT_TRUE(first.has_value());
    EXPECT_EQ(first->id(), "A1");

    auto second = cp.dequeueNextWorkload();
    ASSERT_TRUE(second.has_value());
    EXPECT_EQ(second->id(), "B1");

    auto third = cp.dequeueNextWorkload();
    ASSERT_TRUE(third.has_value());
    EXPECT_EQ(third->id(), "C1");

    auto fourth = cp.dequeueNextWorkload();
    ASSERT_TRUE(fourth.has_value());
    EXPECT_EQ(fourth->id(), "A2");

    EXPECT_FALSE(cp.dequeueNextWorkload().has_value());
  }

TEST(ControlPlaneTest, DispatchesPendingWorkloadWhenSchedulingSucceeds) {
  std::vector<strata::Tenant> tenantList;
  tenantList.emplace_back("tenant-a", strata::ResourceVector(4, 8, 1));
  std::vector<strata::Node> nodeList;
  nodeList.emplace_back("node-a", strata::ResourceVector(8, 16, 2));

  strata::Scheduler scheduler(nodeList);
  strata::ControlPlane cp(tenantList, scheduler);
  cp.enqueue(strata::Workload(
      "job-1", "tenant-a", strata::ResourceVector(2, 4, 1)));

  strata::Node* node = cp.dispatchNext();

  ASSERT_NE(node, nullptr);
  EXPECT_EQ(node->id(), "node-a");
  ASSERT_NE(cp.pending().find("tenant-a"), cp.pending().end());
  EXPECT_TRUE(cp.pending().at("tenant-a").empty());
}

TEST(ControlPlaneTest, RequeuesPendingWorkloadWhenSchedulingFails) {
  std::vector<strata::Tenant> tenantList;
  tenantList.emplace_back("tenant-a", strata::ResourceVector(8, 8, 2));
  std::vector<strata::Node> nodeList;
  nodeList.emplace_back("node-a", strata::ResourceVector(2, 2, 1));

  strata::Scheduler scheduler(nodeList);
  strata::ControlPlane cp(tenantList, scheduler);
  cp.enqueue(strata::Workload(
      "job-1", "tenant-a", strata::ResourceVector(4, 4, 1)));

  strata::Node* node = cp.dispatchNext();

  EXPECT_EQ(node, nullptr);
  const auto& queue = cp.pending().at("tenant-a");
  ASSERT_EQ(queue.size(), 1);
  EXPECT_EQ(queue.front().id(), "job-1");
}

