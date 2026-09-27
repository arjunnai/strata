#pragma once
#include <cstddef>
#include <deque>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

#include "strata/scheduler.h"
#include "strata/tenant.h"
#include "strata/workload.h"
namespace strata {
class ControlPlane {
 public:
  ControlPlane(std::vector<Tenant> tenants, Scheduler scheduler);
  const std::vector<Tenant>& tenants() const;
  const std::unordered_map<std::string, std::deque<Workload>>& pending() const;
  Tenant* findTenant(const std::string& tenant_id);
  Node* submit(const Workload& workload);
  bool enqueue(const Workload& workload);
  std::optional<Workload> dequeueNextWorkload();
  Node* dispatchNext();

 private:
  std::vector<Tenant> tenants_;
  // tenant id -> tenant dq
  std::unordered_map<std::string, std::deque<Workload>> pending_;
  Scheduler scheduler_;
  std::size_t nextTenantIndex_{0};
};

}  // namespace strata

/*
control plane already owns tenants_, pending_ and nextTenantIndex_
*/