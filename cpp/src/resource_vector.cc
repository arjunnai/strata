#include "strata/resource_vector.h"

#include <stdexcept>

strata::ResourceVector::ResourceVector(std::int64_t cpu, std::int64_t memory, std::int64_t gpu)
    : cpu_(cpu), memory_(memory), gpu_(gpu) {
  if (cpu < 0 || memory < 0 || gpu < 0) {
    throw std::invalid_argument("resource values cannot be negative");
  }
}