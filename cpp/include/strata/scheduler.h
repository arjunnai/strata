#pragma once
#include <vector>

#include "strata/node.h"
#include "strata/workload.h"

namespace strata {
enum class PlacementPolicy { FIRST_FIT, BEST_FIT, SPREAD };

class Scheduler {
 public:
  Scheduler(std::vector<Node> nodes, PlacementPolicy policy = PlacementPolicy::FIRST_FIT);
  const std::vector<Node>& nodes() const;
  Node* schedule(const Workload&);

 private:
  std::vector<Node> nodes_;
  PlacementPolicy policy_;
};
}  // namespace strata