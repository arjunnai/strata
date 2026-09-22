#include "strata/node.h"

#include <string>

#include "strata/resource_vector.h"

strata::Node::Node(std::string id, ResourceVector totalCap)
    : id_(id), totalCap_(totalCap), availableCap_(totalCap) {}
const std::string& strata::Node::id() const { return id_; }
const strata::ResourceVector& strata::Node::availableCap() const { return availableCap_; }
const strata::ResourceVector& strata::Node::totalCap() const { return totalCap_; }
bool strata::Node::canFit(const strata::ResourceVector& req) const {
  if (req.workloadFit(availableCap_)) return true;
  return false;
}

bool strata::Node::allocate(const strata::ResourceVector& req){
    if(canFit(req)){
        availableCap_ = availableCap_ - req;
        return true;
    }
    return false;
}