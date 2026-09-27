#include "strata/scheduler.h"

#include <cstddef>
#include <limits>

#include "strata/node.h"
#include "strata/resource_vector.h"

strata::Scheduler::Scheduler(std::vector<Node> nodes, PlacementPolicy policy)
    : nodes_(nodes), policy_(policy) {}
const std::vector<strata::Node>& strata::Scheduler::nodes() const { return nodes_; }

strata::Node* strata::Scheduler::schedule(const strata::Workload& workload) {
  switch (policy_) {
    case strata::PlacementPolicy::FIRST_FIT: {
      for (auto& node : nodes_) {
        if (node.canFit(workload.request())) {
          node.allocate(workload.request());
          return &node;
        }
      }
      break;
    }

    case strata::PlacementPolicy::BEST_FIT: {
      strata::Node* bestNode = nullptr;
      double bestScore = std::numeric_limits<double>::max();
      for (auto& node : nodes_) {
        if (!node.canFit(workload.request())) continue;
        // calc resourcess remaining after placement
        strata::ResourceVector remaining = node.availableCap() - workload.request();
        std::int64_t cpu_rem = remaining.cpu();
        std::int64_t mem_rem = remaining.memory();
        std::int64_t gpu_rem = remaining.gpu();

        double cpu_score =
            node.totalCap().cpu() > 0 ? static_cast<double>(cpu_rem) / node.totalCap().cpu() : 0;
        double mem_score = node.totalCap().memory() > 0
                               ? static_cast<double>(mem_rem) / node.totalCap().memory()
                               : 0;
        double gpu_score =
            node.totalCap().gpu() > 0 ? static_cast<double>(gpu_rem) / node.totalCap().gpu() : 0;

        double totalScore = cpu_score + mem_score + gpu_score;
        if (totalScore < bestScore) {
          bestScore = totalScore;
          bestNode = &node;
        }
      }
      if (bestNode == nullptr) return nullptr;
      bestNode->allocate(workload.request());
      return bestNode;
    }

    case strata::PlacementPolicy::SPREAD: {
      strata::Node* spreadNode = nullptr;
      double bestScore = std::numeric_limits<double>::lowest();
      for (auto& node : nodes_) {
        if (!node.canFit(workload.request())) continue;

        strata::ResourceVector remaining = node.availableCap() - workload.request();
        std::int64_t cpu_rem = remaining.cpu();
        std::int64_t mem_rem = remaining.memory();
        std::int64_t gpu_rem = remaining.gpu();

        double cpu_score =
            node.totalCap().cpu() > 0 ? static_cast<double>(cpu_rem) / node.totalCap().cpu() : 0;
        double mem_score = node.totalCap().memory() > 0
                               ? static_cast<double>(mem_rem) / node.totalCap().memory()
                               : 0;
        double gpu_score =
            node.totalCap().gpu() > 0 ? static_cast<double>(gpu_rem) / node.totalCap().gpu() : 0;

        double totalScore = cpu_score + mem_score + gpu_score;
        if (totalScore > bestScore) {
          bestScore = totalScore;
          spreadNode = &node;
        }
      }
      if (spreadNode == nullptr) return nullptr;
      spreadNode->allocate(workload.request());
      return spreadNode;
    }
  }

  return nullptr;
}