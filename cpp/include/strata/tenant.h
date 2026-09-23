#pragma once
#include "strata/resource_vector.h"
#include <string>
namespace strata {
// how much is this owner allowed to use
class Tenant {
 public:
  Tenant(std::string id, ResourceVector quota);
  const std::string& id() const;
  const ResourceVector& quota() const;
  const ResourceVector& usage() const;
  bool canAdmit(const ResourceVector& req) const;

 private:
  std::string id_;
  ResourceVector quota_;
  ResourceVector usage_;
};
}  // namespace strata