# Product Requirements Document — Strata

## 1. Product statement

Strata is a **multi-tenant resource-management and workload control plane** for two classes of managed workloads:

1. PostgreSQL database clusters
2. LLM inference services

The same platform should make decisions about:
- admission
- fairness
- placement
- capacity
- autoscaling
- health
- failure handling

The database and inference runtimes themselves are intentionally reused rather than reimplemented.

## 2. Primary engineering objective

The project must prove:

> I can design and implement the systems layer that turns raw compute into a multi-tenant managed service.

The project should showcase:
- modern C++
- distributed-systems reasoning
- resource allocation
- scheduling
- control loops
- Kubernetes controllers
- managed stateful systems
- real GPU inference operations

## 3. Language architecture

### C++20 owns policy and algorithms

Implement in C++:
- `ResourceVector`
- `TenantState`
- `NodeState`
- `Workload`
- quota accounting
- dominant-resource-share computation
- fair admission queue
- placement filtering
- placement scoring
- fragmentation calculation
- capacity planner
- autoscaling policies
- policy simulator
- gRPC policy server

### Go owns infrastructure integration

Implement in Go:
- Kubernetes CRDs
- controllers/reconcilers
- status and conditions
- finalizers
- Kubernetes watches
- scheduler-framework adapter
- CloudNativePG resource adapter
- Modal Go SDK adapter
- Prometheus integration
- policy-engine gRPC client

### Python is intentionally tiny

Use Python only for:
- Modal/vLLM service definition
- optional experiment analysis

## 4. Why not 100% C++?

Kubernetes officially maintains a Go client but does not list C++ among its officially supported client libraries. Kubernetes' scheduling plugin interfaces are also Go interfaces compiled directly into `kube-scheduler`.

Therefore, forcing the Kubernetes-facing layer into C++ would optimize for language purity instead of useful systems work.

The goal is to make **C++ the brain** and **Go the adapter layer**.

References:
- https://kubernetes.io/docs/reference/using-api/client-libraries/
- https://kubernetes.io/docs/concepts/scheduling-eviction/scheduling-framework/

## 5. Users

A platform consumer submits a desired workload.

Example database:

```yaml
apiVersion: strata.dev/v1alpha1
kind: StrataWorkload
metadata:
  name: orders-db
spec:
  tenantRef: payments
  type: postgres
  resources:
    cpu: "4"
    memory: "16Gi"
  scaling:
    mode: slo
    minReplicas: 1
    maxReplicas: 4
    targetP99Ms: 75
```

Example inference:

```yaml
apiVersion: strata.dev/v1alpha1
kind: StrataWorkload
metadata:
  name: llama-api
spec:
  tenantRef: ai-team
  type: inference
  inference:
    provider: modal
    model: Qwen/Qwen3-8B
  resources:
    gpu: 1
  scaling:
    mode: slo
    minReplicas: 1
    maxReplicas: 6
    targetTTFTMs: 500
```

## 6. Functional goals

### G1 — Multi-tenant resource model

Model:
- CPU
- memory
- accelerator/GPU units
- workload count

Support:
- tenant hard quotas
- cluster capacity
- reserved resources
- current usage
- queued demand

### G2 — Admission and fairness

Implement:
- FIFO baseline
- weighted round-robin baseline
- DRF-inspired admission policy

A queued workload must expose why it is pending.

### G3 — Placement

Implement:
- feasibility filtering
- resource-fit scoring
- fragmentation-aware scoring
- spread preference for stateful replicas
- pack preference for accelerator jobs

### G4 — Autoscaling

Implement:
- static policy
- threshold-based policy
- SLO-aware policy
- hysteresis
- cooldown
- bounded step size

### G5 — Kubernetes control plane

Implement:
- CRDs
- reconciliation
- idempotency
- status/conditions
- finalizers
- recovery after controller restart

### G6 — Managed PostgreSQL

Use CloudNativePG.

Strata must:
- create/update/delete a Postgres cluster
- observe health
- expose failover state
- scale read replicas
- collect DB metrics

Strata must **not** reimplement PostgreSQL replication.

### G7 — Real inference workload

Use Modal + vLLM.

Strata must:
- identify/control a Modal function
- change autoscaler settings from Go
- generate inference load
- record TTFT/latency/concurrency
- drive scale decisions using the same C++ policy engine

### G8 — Evaluation

Compare policies under repeatable workloads.

Required outputs:
- fairness graph
- scheduling/fragmentation comparison
- DB SLO scaling graph
- inference SLO scaling graph
- failure-recovery measurements

## 7. Non-goals

Do not build:
- a SQL engine
- a storage engine
- Raft/Paxos
- Postgres replication
- Postgres backup format
- Kubernetes from scratch
- kube-scheduler from scratch
- CUDA kernels
- an inference runtime
- tensor-parallel algorithms
- authentication platform
- billing platform
- multi-region production networking

## 8. Success criteria

The project is complete when:

- [ ] C++ simulator runs admission and placement independently of Kubernetes
- [ ] DRF-inspired fair admission works and is unit-tested
- [ ] placement engine exposes explainable scores
- [ ] C++ autoscaler handles multiple policies
- [ ] Go talks to C++ over gRPC
- [ ] Kubernetes controller is idempotent
- [ ] PostgreSQL is provisioned through CloudNativePG
- [ ] database failover is observed correctly
- [ ] custom scheduler adapter uses C++ placement logic
- [ ] Modal/vLLM serves real GPU inference
- [ ] Go can change Modal autoscaler settings
- [ ] policy comparisons have reproducible benchmark data
- [ ] controller/failure chaos tests exist
- [ ] README contains real benchmark numbers and graphs

## 9. Stretch goals

Choose at most one:
- priority/preemption
- prefix/KV-cache-aware routing
- predictive autoscaling
- cost-aware scheduling
- topology-aware accelerator placement on a real GPU Kubernetes cluster

## 10. Career signal

For managed database roles:
- stateful workload orchestration
- Kubernetes control planes
- DB observability
- failover awareness
- multi-tenancy
- autoscaling

For ML inference infrastructure:
- GPU resource model
- admission
- scheduling
- capacity
- inference SLOs
- autoscaling
- vLLM workload experience

For distributed systems roles:
- control loops
- resource allocation
- RPC boundary
- failure recovery
- fairness
- scheduling
