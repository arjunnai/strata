#include "strata/control_plane.h"

#include <cstddef>

#include "strata/node.h"
#include "strata/scheduler.h"

strata::ControlPlane::ControlPlane(std::vector<strata::Tenant> tenants, strata::Scheduler scheduler)
    : tenants_(tenants), scheduler_(scheduler) {}
const std::vector<strata::Tenant>& strata::ControlPlane::tenants() const { return tenants_; }

strata::Tenant* strata::ControlPlane::findTenant(const std::string& tenant_id) {
  for (auto& tenant : tenants_) {
    if (tenant.id() == tenant_id) {
      return &tenant;
    }
  }
  return nullptr;
}

strata::Node* strata::ControlPlane::submit(const strata::Workload& workload) {
  // workload arrives -> findtenant (workload's tenant id)
  strata::Tenant* tenant = findTenant(workload.tenant_id());
  // check if tenant wasn't found
  if (tenant == nullptr) return nullptr;
  // check tenant has enough quota
  if (!tenant->canAdmit(workload.request())) {
    return nullptr;
  }

  strata::Node* node = scheduler_.schedule(workload);
  if (node == nullptr) return nullptr;

  // if tenant has enough quota
  tenant->admit(workload.request());
  // scheduler can scheule workload
  return node;
}