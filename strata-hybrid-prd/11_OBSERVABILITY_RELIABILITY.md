# 11 — Observability, Failure Handling, and Chaos

## Goal

Make every important platform decision observable.

## 1. Control-plane metrics

Expose:

```text
strata_reconcile_total
strata_reconcile_errors_total
strata_reconcile_duration_seconds
strata_policy_rpc_duration_seconds
strata_policy_rpc_errors_total
```

## 2. Admission metrics

```text
strata_admission_total{decision,reason}
strata_queue_wait_seconds
strata_tenant_dominant_share
strata_tenant_cpu_usage
strata_tenant_memory_usage
strata_tenant_gpu_usage
```

## 3. Scheduling metrics

```text
strata_scheduler_filter_total{result}
strata_scheduler_score_duration_seconds
strata_pending_unschedulable
```

Detailed score breakdown can live in structured logs.

## 4. Scaling metrics

```text
strata_scale_decisions_total{direction,reason}
strata_desired_capacity
strata_actual_capacity
strata_slo_error_ratio
```

## 5. Workload metrics

Database:
- P99 latency
- TPS
- errors
- replicas
- failover state

Inference:
- TTFT
- E2E latency
- errors
- concurrency
- capacity

## 6. Dashboard

One Grafana dashboard:

1. workload states
2. queued workloads
3. tenant shares
4. resource utilization
5. scale decisions
6. desired vs actual
7. DB P99
8. inference TTFT
9. controller errors

## 7. Chaos scenarios

### C++ policy engine killed

Expected:
- existing workloads remain
- new decisions pause
- controller reports policy unavailable
- retry succeeds after restart

### Go controller killed

Expected:
- leader/restart recovery
- reconciliation resumes
- no duplicate resource

### Postgres primary killed

Expected:
- CNPG failover
- Strata reflects state
- traffic recovers

### Kubernetes node removed

Expected:
- capacity snapshot changes
- pending decisions change

### Inference endpoint fails

Expected:
- unhealthy status
- observable errors
- no false Ready state

## 8. Reliability invariants

- tenant usage is never double-counted
- deletion eventually releases resources
- scale action never exceeds min/max
- repeated reconcile does not duplicate provider resource
- stale provider state does not silently overwrite newer desired state
- policy engine failure cannot destroy healthy workload

## 9. Logging

Every decision log should include:
- tenant
- workload
- policy
- input summary
- output
- reason
- reconcile ID/correlation ID
