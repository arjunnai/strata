# 03 — Simulator, Admission, and Fair Sharing

## Goal

Build a deterministic simulator so policies can be understood and benchmarked without infrastructure noise.

## 1. Simulator input

Example conceptual format:

```json
{
  "capacity": {
    "cpu_millis": 32000,
    "memory_bytes": 137438953472,
    "gpu_units": 4
  },
  "tenants": [],
  "running": [],
  "queue": []
}
```

Do not over-engineer the parser initially.

Hard-coded test scenarios are acceptable before JSON.

## 2. Simulator events

Support:

```text
SubmitWorkload
CompleteWorkload
AdvanceTime
MetricSample
NodeAdd
NodeRemove
```

MVP only requires submit/complete.

## 3. FIFO baseline

Implement FIFO first.

Why:
- validates resource accounting
- gives a comparison baseline
- exposes starvation/fairness issues

## 4. Weighted round-robin baseline

Optional but useful.

Maintain tenant queues.

Rotate among tenants with queued work.

## 5. DRF-inspired fair sharing

For each tenant:

```text
cpu_share    = tenant_cpu    / cluster_cpu
memory_share = tenant_memory / cluster_memory
gpu_share    = tenant_gpu    / cluster_gpu

dominant_share =
    max(cpu_share, memory_share, gpu_share)
```

Weighted form:

```text
effective_share =
    dominant_share / tenant_weight
```

Prefer work from the tenant with the lowest effective dominant share, provided the workload fits available capacity.

## 6. Important edge cases

### Zero-capacity resource

If cluster has zero GPUs:
- a tenant using zero GPUs should not divide by zero
- a GPU-requesting workload is infeasible

### Workload larger than total cluster

Mark:
```text
UnschedulableRequest
```

Do not let it block every other queued workload forever.

### Tenant below fair share but request does not fit

Consider next eligible tenant.

### Multiple jobs from one tenant

Define queue ordering explicitly.

MVP:
- FIFO within tenant

## 7. Fairness vs utilization

Expect cases where pure fairness and maximum utilization conflict.

Do not hide that.

Example:
- low-share tenant requests a large GPU job that cannot currently fit
- another tenant has a small CPU job that can fit

Decide:
- strict fairness blocks
- work-conserving fairness skips temporarily

Recommended:
- work-conserving policy
- document the tradeoff

## 8. Required tests

- equal tenants converge toward equal dominant shares
- weighted tenant receives proportionally different share
- one tenant cannot exceed hard quota
- no starvation in feasible repeated workload
- oversized head-of-line job does not deadlock queue
- GPU-free cluster handles CPU workloads
- release changes next admission decision

## 9. Benchmark

Generate synthetic workload traces.

Compare:
- FIFO
- round robin
- DRF-inspired

Measure:
- average queue time
- P95 queue time
- admitted jobs
- utilization
- dominant-share variance
- starvation count

## 10. Learning task

Before implementing DRF:
- calculate three examples by hand
- predict the next admitted tenant
- then make the simulator reproduce your hand calculation

Do not code the algorithm until the examples make sense.
