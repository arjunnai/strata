#include "strata/tenant.h"

#include "strata/resource_vector.h"

strata::Tenant::Tenant(std::string id, ResourceVector quota)
    : id_(id), quota_(quota), usage_(0, 0, 0) {}

const std::string& strata::Tenant::id() const { return id_; }

const strata::ResourceVector& strata::Tenant::quota() const { return quota_; }
const strata::ResourceVector& strata::Tenant::usage() const { return usage_; }
bool strata::Tenant::canAdmit(const strata::ResourceVector& req) const {
  ResourceVector res = usage_ + req;
  return res.workloadFit(quota_);
}

bool strata::Tenant::admit(const strata::ResourceVector& req) {
  if (canAdmit(req)) {
    usage_ = usage_ + req;
    return true;
  }
  return false;
}
bool strata::Tenant::release(const strata::ResourceVector& req) {
  if (req.workloadFit(usage_)) {
    usage_ = usage_ - req;
    return true;
  }
  return false;
}