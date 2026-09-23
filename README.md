# Strata

A multi-tenant control plane and resource manager for managed database
and inference workloads.

## Status

Early development. Core resource modeling, node allocation, first-fit scheduling,
and tenant admission control are implemented.

## Build

```bash
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

## Current Progress

Strata currently has the first version of its core resource-management model.

Implemented so far:

- `ResourceVector`
  - CPU, memory, and GPU resource representation
  - Validation preventing negative resource quantities
  - Resource addition and subtraction
  - Fit checks between requested and available capacity

- `Workload`
  - Workload ID
  - Tenant ownership
  - Requested resource vector

- `Node`
  - Total and available capacity tracking
  - Resource fit checks
  - Resource allocation with capacity updates

- `Scheduler`
  - Maintains a collection of nodes
  - First-fit workload placement
  - Allocates resources on the first node capable of running a workload
  - Returns no placement when cluster capacity is insufficient

- `Tenant`
  - Tenant ID
  - Resource quota
  - Current resource usage
  - Admission checks against tenant quota
  

- GoogleTest coverage for resource arithmetic, workload state, node allocation,
  scheduling behavior, and tenant admission control

## Example Scheduling Flow

```cpp
strata::Workload workload(
    "job-1",
    "team-a",
    strata::ResourceVector(4, 8, 1));

std::vector<strata::Node> nodes;
nodes.emplace_back("node-1", strata::ResourceVector(2, 4, 1));
nodes.emplace_back("node-2", strata::ResourceVector(8, 16, 2));

strata::Scheduler scheduler(nodes);

strata::Node* node = scheduler.schedule(workload);

if (node != nullptr) {
    // workload was allocated to the selected node
}
```

## Roadmap

Next:

- Update tenant usage when workloads are admitted
- Release resources when workloads complete
- Tenant accounting and quota enforcement
- Scheduling policies beyond first-fit
- Multi-tenant fairness and DRF-inspired admission
- Placement scoring and fragmentation awareness
- Large-scale workload simulator
- Autoscaling and capacity policies
- gRPC policy-engine interface
- Kubernetes control-plane integration
- Managed PostgreSQL and inference-provider integrations
