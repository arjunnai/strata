#pragma once
#include <string>

#include "strata/resource_vector.h"
namespace strata {
class Node {
 public:
  Node(std::string id, ResourceVector totalCap);
  const std::string& id() const;
  const ResourceVector& totalCap() const;
  const ResourceVector& availableCap() const;

  bool canFit(const ResourceVector& req) const;
  bool allocate(const ResourceVector& req);

 private:
  std::string id_;
  ResourceVector totalCap_;
  ResourceVector availableCap_;
};

}  // namespace strata