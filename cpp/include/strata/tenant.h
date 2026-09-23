#pragma once
#include <string>

#include "strata/resource_vector.h"
namespace strata {
// how much is this owner allowed to use
class Tenant {
 public:
  Tenant(std::string id, ResourceVector quota);
  const std::string& id() const;
  const ResourceVector& quota() const;
  const ResourceVector& usage() const;
  bool canAdmit(const ResourceVector& req) const;
  bool admit(const ResourceVector& req);
  bool release(const ResourceVector& req);

 private:
  std::string id_;
  ResourceVector quota_;
  ResourceVector usage_;
};
}  // namespace strata