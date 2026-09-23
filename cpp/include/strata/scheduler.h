#pragma once
#include <vector>

#include "strata/node.h"
#include "strata/workload.h"

namespace strata {
class Scheduler {
 public:
  Scheduler(std::vector<Node> nodes);
  const std::vector<Node>& nodes() const;
  Node* schedule(const Workload&);

 private:
  std::vector<Node> nodes_;
};
}  // namespace strata