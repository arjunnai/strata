#include "strata/scheduler.h"

strata::Scheduler::Scheduler(std::vector<Node> nodes) : nodes_(nodes) {}
const std::vector<strata::Node>& strata::Scheduler::nodes() const { return nodes_; }

strata::Node* strata::Scheduler::schedule(const strata::Workload& workload){
    for(auto& node : nodes_){
        if(node.canFit(workload.request())) {
            node.allocate(workload.request());
            return &node;
        }
    }
    return nullptr;
}