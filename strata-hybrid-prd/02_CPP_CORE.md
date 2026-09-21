# 02 — C++ Core Domain Model

## Goal

Build the resource-management brain as a normal C++ library **before** introducing RPC or Kubernetes.

## 1. Suggested toolchain

- C++20
- CMake
- GoogleTest
- clang-format
- clang-tidy if time allows

Do not introduce a huge dependency stack.

## 2. Core types

Design these yourself before reading implementation examples.

### ResourceVector

Represents resources in normalized units:

```text
cpu_millis
memory_bytes
gpu_units
```

Operations needed:
- addition
- subtraction
- comparison/fit
- non-negative validation
- ratio against capacity

Questions to solve:
- should subtraction be allowed to go negative?
- should values be signed?
- how do you represent absent GPU capacity?
- how do you handle division by zero?

### ResourceQuota

```text
hard_limit
current_usage
reserved_usage
```

Invariant:

```text
current_usage + reserved_usage <= hard_limit
```

### Tenant

Fields:
- ID
- weight
- quota
- current usage
- admitted workloads

### Workload

Fields:
- workload ID
- tenant ID
- workload class
- requested resources
- replica semantics
- placement preference
- priority
- min/max capacity if scalable

### Node

Fields:
- node ID
- capacity
- allocated
- labels / accelerator type
- failure domain

### ClusterSnapshot

Contains:
- nodes
- tenants
- queued workloads
- active workloads

## 3. Required invariants

Write tests for these before higher-level policies.

### Resource conservation

```text
free + allocated == capacity
```

for every modeled resource.

### Tenant hard quota

No admitted allocation may exceed tenant quota.

### Non-negative resource state

After every action:

```text
free >= 0
usage >= 0
```

### Unique workload accounting

A workload contributes to tenant usage exactly once.

### Release correctness

Removing a workload restores its resource allocation.

## 4. Recommended design style

Prefer:
- small value types
- const-correct interfaces
- explicit results
- RAII
- deterministic pure calculations where possible

Avoid:
- global mutable state
- singleton managers
- hidden thread pools
- premature templates
- overly generic plugin frameworks

## 5. Error modeling

Prefer structured errors:

```text
InvalidResourceRequest
UnknownTenant
QuotaExceeded
InsufficientCapacity
InvalidNodeState
```

Do not make policy code throw exceptions for expected control-flow decisions.

A failed admission is a **decision**, not necessarily an exception.

## 6. Explainability

All major policy results should return a reason.

Example:

```text
AdmissionDecision
  admitted = false
  reason = TenantQuotaExceeded
  details:
    gpu_requested = 2
    gpu_used = 3
    gpu_limit = 4
```

Placement:

```text
PlacementDecision
  node = worker-3
  score = 82
  components:
    fit = 30
    fragmentation = 28
    topology = 14
    preference = 10
```

This helps:
- tests
- debugging
- metrics
- interviews

## 7. First exercise

Before building the full model:

1. support CPU only
2. implement `Fits()`
3. add memory
4. add GPU
5. add tenant quota
6. add allocation/release

Do not skip directly to DRF.

## 8. What you should learn while writing this

C++ practice goals:
- value semantics
- ownership
- const correctness
- STL containers
- `std::optional`
- variants/enums
- strong typing
- testing
- API design

This project should improve your C++ fluency, not merely produce C++ files.
