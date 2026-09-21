# 04 — C++ Placement Engine

## Goal

Given an admitted workload and candidate nodes:

1. filter infeasible nodes
2. score feasible nodes
3. select placement
4. explain the decision

## 1. Interface

Conceptually:

```text
Filter(workload, node) -> Feasibility
Score(workload, node, cluster) -> ScoreBreakdown
Place(workload, cluster) -> PlacementDecision
```

Do not copy Kubernetes types into this layer.

## 2. Filter stage

Reject node if:
- CPU does not fit
- memory does not fit
- GPU units do not fit
- required GPU type does not match
- required label missing
- anti-affinity rule violated

Return a reason for each rejection.

## 3. Score components

### Resource fit

Question:
- should you prefer empty nodes or full nodes?

Implement both strategies experimentally:
- spread
- bin-pack

### Fragmentation

A simple first metric:

```text
remaining_cpu_fraction
remaining_memory_fraction
remaining_gpu_fraction
```

Penalize placements that leave unusable resource shapes.

Example:
- a 1-GPU job should not necessarily consume the only node with 8 contiguous GPU units if a 1-GPU node exists

### Failure-domain spread

For database replicas:
- prefer different nodes
- optionally prefer different zones

### Pack preference

For multi-accelerator workloads:
- prefer concentration when appropriate

## 4. Weighted score

Example:

```text
total =
    0.35 * fit
  + 0.35 * fragmentation
  + 0.20 * topology
  + 0.10 * workload_preference
```

Do not obsess over perfect weights.

The benchmark should show sensitivity.

## 5. Explainability

Required:

```text
node-a:
  feasible = true
  fit = 27
  fragmentation = 18
  topology = 30
  workload = 8
  total = 83

node-b:
  feasible = false
  reason = InsufficientMemory
```

## 6. Multi-replica placement

For a workload with N replicas:

Simple MVP:
- place sequentially
- update a temporary cluster snapshot after each placement

Do not solve globally optimal bin packing.

Document that greedy placement can be suboptimal.

## 7. Complexity

For:
- W workloads
- N nodes

Simple placement is roughly:

```text
O(W * N)
```

plus score computation.

This is completely adequate for the project.

## 8. Tests

- exact-fit node
- insufficient CPU
- insufficient memory
- GPU mismatch
- spread database replicas
- pack accelerator replicas
- fragmentation changes winner
- tie-breaking deterministic
- sequential replica placement updates resources

## 9. Experiment

Create mixed traces:
- 1 CPU / 2 GB
- 8 CPU / 32 GB
- 1 GPU
- 2 GPU
- 4 GPU

Compare:
- first-fit
- best-fit
- Strata weighted score

Measure:
- successful placements
- stranded resources
- utilization
- pending workloads
