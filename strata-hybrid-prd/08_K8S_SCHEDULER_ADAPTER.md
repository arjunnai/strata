# 08 — Kubernetes Scheduler Adapter

## Goal

Connect the C++ placement logic to a real Kubernetes scheduling extension point.

Kubernetes Scheduling Framework:
https://kubernetes.io/docs/concepts/scheduling-eviction/scheduling-framework/

The framework exposes extension points including `Filter` and `Score`; plugin interfaces are Go interfaces compiled into the scheduler.

## 1. Architecture

```text
kube-scheduler
     |
     v
Go Strata plugin
     |
translate Pod/Node -> Strata model
     |
     v
C++ policy engine
     |
placement score / feasibility
     |
     v
Go returns framework result
```

## 2. MVP extension points

Implement:
- `Filter`
- `Score`

Do not begin with:
- Reserve
- Permit
- PreBind
- Bind
- PostFilter/preemption

## 3. Filter

Go plugin:
1. receives Pod + NodeInfo
2. builds resource request
3. builds normalized node snapshot
4. calls C++ placement feasibility
5. returns schedulable/unschedulable

## 4. Score

Go plugin:
1. calls C++ score
2. normalizes if required
3. maps to Kubernetes scheduler score range
4. emits structured debug log

## 5. Workload metadata

Use annotations/labels to communicate:
- workload class
- placement preference
- tenant
- accelerator type

Example:

```text
strata.dev/class=postgres-replica
strata.dev/placement=spread
```

## 6. Synthetic GPU resources

If Kubernetes test cluster has no GPU:

Expose synthetic extended resources such as:

```text
strata.dev/gpu: 4
```

Use them only to test scheduling logic.

Do not claim real GPU execution from this environment.

Real inference compute happens on Modal.

## 7. Experiments

Compare:
- normal/default scheduler behavior
- Strata plugin

Use traces designed to cause fragmentation.

Measure:
- schedulable jobs
- utilization
- stranded resources
- scheduler latency

## 8. Scope warning

A full external scheduler would consume too much time.

The plugin is enough to prove:
- scheduler framework understanding
- resource normalization
- policy integration
- placement logic
