#pragma once
#include <cstdint>
namespace strata {
class ResourceVector {
 public:
  ResourceVector(int64_t cpu, int64_t memory, int64_t gpu);

 private:
  int64_t cpu_;
  int64_t memory_;
  int64_t gpu_;
};
}  // namespace strata