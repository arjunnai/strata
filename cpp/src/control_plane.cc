#include "strata/control_plane.h"

#include <cstddef>
#include <optional>

#include "strata/node.h"
#include "strata/scheduler.h"
#include "strata/tenant.h"
#include "strata/workload.h"

strata::ControlPlane::ControlPlane(std::vector<strata::Tenant> tenants, strata::Scheduler scheduler)
    : tenants_(tenants), scheduler_(scheduler) {}
const std::vector<strata::Tenant>& strata::ControlPlane::tenants() const { return tenants_; }
const std::unordered_map<std::string, std::deque<strata::Workload>>& strata::ControlPlane::pending()
    const {
  return pending_;
}
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

bool strata::ControlPlane::enqueue(const strata::Workload& workload) {
  strata::Tenant* tenant = findTenant(workload.tenant_id());
  if (tenant == nullptr) return false;
  pending_[tenant->id()].push_back(workload);
  return true;
}

std::optional<strata::Workload> strata::ControlPlane::dequeueNextWorkload() {
  if (tenants_.empty()) return std::nullopt;
  for (size_t i = 0; i < tenants_.size(); i++) {
    Tenant& tenant = tenants_[nextTenantIndex_];
    // get the pending workload queue for each tenant
    auto& q = pending_[tenant.id()];
    if (q.empty()) {
      nextTenantIndex_ = (nextTenantIndex_ + 1) % tenants_.size();
      continue;
    }
    Workload workload = q.front();
    q.pop_front();
    nextTenantIndex_ = (nextTenantIndex_ + 1) % tenants_.size();
    return workload;
  }

  return std::nullopt;
}

strata::Node* strata::ControlPlane::dispatchNext() {
  auto workload = dequeueNextWorkload();
  if (!workload.has_value()) return nullptr;
  auto* node = submit(*workload);
  if (node) {
    return node;
  }
  // schedule failed
  pending_[workload->tenant_id()].push_front(*workload);
  return nullptr;
}