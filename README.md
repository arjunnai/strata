# Strata

A multi-tenant control plane and resource manager for managed database
and inference workloads.

## Status

Early development.

## Build

```bash
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure

## Current Progress

Strata is currently in the resource-modeling phase.

Implemented so far:

- `ResourceVector` abstraction for CPU, memory, and GPU resources
- Validation preventing negative resource values
- Read-only accessors for resource quantities
- Workload-fit checks against available capacity
- Resource subtraction for tracking remaining capacity
- GoogleTest coverage for validation, boundary cases, fit checks, and subtraction

### Example

```cpp
ResourceVector request(4, 16, 1);
ResourceVector capacity(8, 20, 5);

if (request.workloadFit(capacity)) {
    ResourceVector remaining = capacity - request;
}
```

This resource model will become the foundation for Strata's admission control, placement, tenant accounting, fairness, and autoscaling logic.