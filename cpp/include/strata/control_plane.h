#pragma once
#include <vector>

#include "strata/scheduler.h"
#include "strata/tenant.h"
namespace strata {
class ControlPlane {
 public:
  ControlPlane(std::vector<Tenant> tenants, Scheduler scheduler);
  const std::vector<Tenant>& tenants() const;
  Tenant* findTenant(const std::string& tenant_id);
  Node* submit(const Workload& workload);

 private:
  std::vector<Tenant> tenants_;
  Scheduler scheduler_;
};

}  // namespace strata