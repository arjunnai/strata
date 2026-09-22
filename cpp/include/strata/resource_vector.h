#pragma once
#include <cstdint>
namespace strata {
class ResourceVector {
 public:
  ResourceVector(std::int64_t cpu, std::int64_t memory, std::int64_t gpu);
  std::int64_t cpu() const;
  std::int64_t memory() const;
  std::int64_t gpu() const;
  // does a workload fit
  bool workloadFit(const ResourceVector& capacity) const;
  //how much resources remain
  ResourceVector operator-(const ResourceVector& capacity) const;
  //how much resource is a tenant or group of workloads using in total
  ResourceVector operator+(const ResourceVector& other) const;

 private:
  std::int64_t cpu_;
  std::int64_t memory_;
  std::int64_t gpu_;
};
}  // namespace strata