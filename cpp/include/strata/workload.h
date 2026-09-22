#pragma once
#include <string>
#include "strata/resource_vector.h"
namespace strata {
class Workload {
 public:
    Workload(std::string id, std::string tenant_id, ResourceVector request);
    const std::string& id() const;
    const std::string& tenant_id() const;
    const ResourceVector& request() const;
 private:
  std::string id_;
  std::string tenant_id_;
  ResourceVector request_;
};

}  // namespace strata