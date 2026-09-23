#include "strata/tenant.h"

#include "strata/resource_vector.h"

strata::Tenant::Tenant(std::string id, ResourceVector quota)
    : id_(id), quota_(quota), usage_(0, 0, 0) {}

const std::string& strata::Tenant::id() const { return id_; }

const strata::ResourceVector& strata::Tenant::quota() const { return quota_; }
const strata::ResourceVector& strata::Tenant::usage() const { return usage_; }