#include "strata/resource_vector.h"

#include <stdexcept>

strata::ResourceVector::ResourceVector(std::int64_t cpu, std::int64_t memory, std::int64_t gpu)
    : cpu_(cpu), memory_(memory), gpu_(gpu) {
  if (cpu < 0 || memory < 0 || gpu < 0) {
    throw std::invalid_argument("resource values cannot be negative");
  }
}

std::int64_t strata::ResourceVector::cpu() const { return cpu_; }
std::int64_t strata::ResourceVector::memory() const { return memory_; }
std::int64_t strata::ResourceVector::gpu() const { return gpu_; }

bool strata::ResourceVector::workloadFit(const strata::ResourceVector& capacity) const {
  if (cpu() <= capacity.cpu() && memory() <= capacity.memory() && gpu() <= capacity.gpu()) {
    return true;
  }
  return false;
}

strata::ResourceVector strata::ResourceVector::operator-(
    const strata::ResourceVector& request) const {
  return strata::ResourceVector{cpu() - request.cpu(), memory() - request.memory(),
                                       gpu() - request.gpu()};
}