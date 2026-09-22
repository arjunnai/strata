#pragma once
#include <cstdint>
namespace strata {
class ResourceVector {
 public:
  ResourceVector(std::int64_t cpu, std::int64_t memory, std::int64_t gpu);
  std::int64_t cpu() const;
  std::int64_t memory() const;
  std::int64_t gpu() const;
  bool workloadFit(const ResourceVector& capacity) const;
  ResourceVector operator-(const ResourceVector& capacity) const;

 private:
  std::int64_t cpu_;
  std::int64_t memory_;
  std::int64_t gpu_;
};
}  // namespace strata