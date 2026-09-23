#include "strata/scheduler.h"

#include <gtest/gtest.h>

#include "strata/node.h"
#include "strata/resource_vector.h"
#include "strata/workload.h"

TEST(SchedulerTest, StoresConstructorValues) {
  strata::Node n1("node-1", strata::ResourceVector(8, 16, 1));
  strata::Node n2("node-2", strata::ResourceVector(4, 8, 2));

  std::vector<strata::Node> nodeList;
  nodeList.push_back(n1);
  nodeList.push_back(n2);

  strata::Scheduler s1(nodeList);

  EXPECT_EQ(s1.nodes().size(), 2);
  EXPECT_EQ(s1.nodes()[0].id(), "node-1");
  EXPECT_EQ(s1.nodes()[1].id(), "node-2");
}

TEST(SchedulerTest, SchedulesWorkloadOnFirstNodeThatFits) {
  std::vector<strata::Node> nodeList;
  nodeList.emplace_back("small", strata::ResourceVector(2, 4, 1));
  nodeList.emplace_back("large", strata::ResourceVector(8, 16, 2));

  strata::Scheduler scheduler(nodeList);

  strata::Workload workload(
      "job-1",
      "team-a",
      strata::ResourceVector(4, 8, 1));

  strata::Node* scheduledNode = scheduler.schedule(workload);

  ASSERT_NE(scheduledNode, nullptr);
  EXPECT_EQ(scheduledNode->id(), "large");

  EXPECT_EQ(scheduledNode->availableCap().cpu(), 4);
  EXPECT_EQ(scheduledNode->availableCap().memory(), 8);
  EXPECT_EQ(scheduledNode->availableCap().gpu(), 1);
}

TEST(SchedulerTest, ReturnsNullWhenNoNodeCanFitWorkload) {
  std::vector<strata::Node> nodeList;
  nodeList.emplace_back("node-1", strata::ResourceVector(2, 4, 1));

  strata::Scheduler scheduler(nodeList);

  strata::Workload workload(
      "job-2",
      "team-a",
      strata::ResourceVector(4, 8, 2));

  EXPECT_EQ(scheduler.schedule(workload), nullptr);
}