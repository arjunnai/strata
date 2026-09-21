# 07 — Go Kubernetes Control Plane

## Goal

Learn controllers and reconciliation without making Go the main algorithm language.

Use:
- Kubebuilder
- controller-runtime
- client-go

Kubebuilder emphasizes that reconciliation is an ongoing desired-state synchronization loop and should be idempotent:
https://book.kubebuilder.io/reference/good-practices

## 1. Before Strata: build an Echo controller

Do this as a learning exercise.

CRD:

```yaml
kind: Echo
spec:
  replicas: 3
```

Controller:
- creates N trivial Pods/Deployments
- if one disappears, restore count
- if replicas changes, converge
- if controller restarts, converge

Do not add gRPC yet.

The point is to understand:
- watch
- reconcile
- desired state
- observed state
- owner references
- status
- idempotency

## 2. Strata CRDs

### Tenant

Spec:
- quota
- weight
- max workloads

Status:
- current usage
- dominant share
- conditions

### StrataWorkload

Spec:
- tenant reference
- type
- resources
- scaling
- provider-specific config

Status:
- lifecycle phase
- admission state
- actual capacity
- health
- scale decision
- conditions

## 3. Workload reconciliation loop

Conceptually:

```text
fetch workload
   |
validate
   |
observe provider
   |
build policy snapshot
   |
call C++ policy engine
   |
execute action if needed
   |
update status
```

## 4. Idempotency problem to solve

Scenario:

1. controller asks provider to create DB
2. provider succeeds
3. controller crashes before status update
4. reconciliation runs again

Correct behavior:
- discover existing named DB
- do not create duplicate
- converge status

Make this a test.

## 5. Finalizers

Needed for external/provider resources.

Deletion:

```text
deletionTimestamp set
      |
provider cleanup
      |
verify cleanup
      |
remove finalizer
```

## 6. Conditions

Use conditions such as:
- Valid
- Admitted
- Provisioned
- Ready
- SLOHealthy
- Degraded

Every false condition should have:
- reason
- message
- transition time

## 7. Leader election

Run at least two controller replicas once.

Kill leader.

Verify:
- new leader takes over
- no duplicate actions

## 8. What Go should not do

Do not duplicate:
- DRF
- placement algorithm
- autoscaling decision logic

Go should normalize inputs, call C++, and apply decisions.
