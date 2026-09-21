# 01 — Architecture and Boundaries

## 1. Topology

```text
                   +-------------------------+
                   | Kubernetes API Server   |
                   +-----------+-------------+
                               |
             watch CRDs        |        scheduler extension
                               |
                   +-----------v-------------+
                   |     Go Control Plane    |
                   +-------------------------+
                   | Tenant Controller       |
                   | Workload Controller     |
                   | Provider Adapters       |
                   | Scheduler Adapter       |
                   | Metrics                 |
                   +-----------+-------------+
                               |
                              gRPC
                               |
                   +-----------v-------------+
                   |    C++ Policy Engine    |
                   +-------------------------+
                   | Resource Accounting     |
                   | Admission/Fairness      |
                   | Placement               |
                   | Autoscaling             |
                   | Capacity Planning       |
                   +-----------+-------------+
                               |
                       deterministic output
                               |
        +----------------------+---------------------+
        |                                            |
        v                                            v
+-------------------+                       +-------------------+
| CloudNativePG     |                       | Modal + vLLM      |
| PostgreSQL        |                       | GPU inference     |
+-------------------+                       +-------------------+
```

## 2. Architectural principle

**Policies must not depend directly on Kubernetes objects.**

Bad C++ API:

```text
Evaluate(kubernetes::Pod pod, kubernetes::Node node)
```

Good C++ API:

```text
Evaluate(Workload workload, NodeState node)
```

Why:
- unit testing becomes simple
- simulator and Kubernetes use the same engine
- policy code remains infrastructure-agnostic
- Modal workload can reuse the same abstractions

## 3. C++ process

Run one service:

```text
strata-policy-engine
```

Responsibilities:
- maintain or receive snapshots of policy-relevant state
- calculate decisions
- return explainable results

Do not make it responsible for:
- Kubernetes API calls
- Modal calls
- database API calls

## 4. Go controller

Responsibilities:
- observe infrastructure
- normalize state
- call C++ policy engine
- execute approved actions
- report status

This is a classic control-plane split:

```text
observe
  |
normalize
  |
decide  ----> C++
  |
act     ----> Go provider
  |
observe again
```

## 5. State ownership

### Kubernetes is source of truth for desired state

CRDs contain:
- tenant configuration
- workload desired state
- scaling config

### Infrastructure is source of truth for actual state

Examples:
- Pods
- CNPG Cluster status
- Modal autoscaler settings
- metrics

### C++ is not a durable source of truth

Policy engine can rebuild state after restart.

Prefer:
- stateless request/response APIs initially

Only add internal cached state if profiling demonstrates a need.

## 6. Consistency model

Eventually consistent.

The platform converges:

```text
desired = 4 replicas
actual  = 2 replicas

controller observes
      |
policy approves
      |
provider scales
      |
actual becomes 4
```

Do not promise instantaneous consistency.

## 7. Failure boundaries

Plan for:
- C++ service unavailable
- Go controller restart
- Kubernetes watch event duplicated
- provider call times out after succeeding
- stale metric sample
- node disappears
- Postgres primary fails
- inference endpoint unhealthy

## 8. Fallback behavior

If policy engine is unavailable:
- do not make speculative scale/admission decisions
- retain existing healthy workload
- mark control-plane decision as degraded
- retry

Availability of existing workloads should not depend on every control-plane decision succeeding.

## 9. Why gRPC?

The C++/Go boundary is deliberate:
- language-neutral schema
- typed requests
- easy local development
- easy mocks
- realistic service boundary

Official C++ gRPC documentation:
https://grpc.io/docs/languages/cpp/
