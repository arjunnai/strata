# 13 — Testing Strategy

## Goal

Use tests to learn systems invariants, not just to raise coverage.

## 1. C++ unit tests

### ResourceVector

- add/subtract
- exact fit
- insufficient resource
- zero capacity
- overflow/invalid values

### Tenant accounting

- admit once
- release once
- duplicate admission rejected
- quota boundary
- weighted share

### Admission

- FIFO
- DRF ordering
- oversized workload
- head-of-line blocking
- starvation scenario

### Placement

- infeasible node
- tie
- spread
- pack
- fragmentation
- GPU type

### Autoscaling

- one noisy sample
- sustained breach
- cooldown
- clamp
- scale-down hysteresis
- invalid metrics

## 2. Property-style invariants

Even without a property-testing framework, generate random states and assert:

```text
allocation never exceeds node capacity
tenant never exceeds hard quota
release never creates negative usage
scale output always within [min,max]
```

This is high-value.

## 3. C++/Go integration tests

Run policy service.

Go client:
- admission happy path
- timeout
- invalid request
- unavailable service
- repeated identical call

## 4. Controller tests

Test:
- initial provisioning
- repeated reconcile
- provider already exists
- provider timeout
- delete/finalizer
- status update
- restart/reconcile

## 5. Failure-in-the-middle tests

Very important:

```text
provider create succeeds
controller crashes before status
```

Then restart.

Expected:
- observe existing resource
- no duplicate

## 6. Scheduler tests

Use fake nodes/pods.

Verify:
- C++ score mapped correctly
- rejection reason surfaced
- deterministic ordering

## 7. End-to-end tests

Minimal:
- create Tenant
- create StrataWorkload
- wait until Ready
- update scale config
- delete workload
- verify cleanup

## 8. Test naming

Name tests around behavior:

Good:
```text
DoesNotAdmitTenantBeyondGPUQuota
PrefersLowerDominantShareWhenBothJobsFit
DoesNotDuplicateProviderResourceAfterReconcileRetry
```

Bad:
```text
TestAdmission1
TestController2
```
