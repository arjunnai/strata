# 09 — PostgreSQL Provider via CloudNativePG

## Goal

Use a production-style Postgres operator so your project focuses on managed-service control plane behavior rather than rebuilding HA.

CloudNativePG supports automated failover and rolling operations.

References:
- https://cloudnative-pg.io/
- https://cloudnative-pg.io/docs/1.27/failover/
- https://cloudnative-pg.io/docs/current/rolling_update/

## 1. Strata responsibilities

Strata:
- admission
- resource quota
- desired DB service abstraction
- creating CNPG `Cluster`
- health translation
- read-replica scaling
- SLO policy
- metrics
- failure visibility

CloudNativePG:
- PostgreSQL process lifecycle
- replication
- primary selection
- failover execution
- rolling instance update

## 2. MVP provider methods

Conceptually:

```text
Ensure
Observe
ScaleReadCapacity
Delete
```

## 3. Resource mapping

Strata spec:

```text
cpu
memory
instances
storage
```

Map into CNPG cluster fields.

Keep the mapping visible and documented.

## 4. Health translation

CNPG state -> Strata state:

```text
healthy     -> Ready
failover    -> Degraded/FailingOver
not enough ready instances -> Degraded
unavailable -> Failed/Unavailable
```

## 5. Scaling semantics

For MVP:

```text
database horizontal scaling =
read-capacity / replica scaling
```

Do not pretend the primary write path is horizontally scalable.

Document this carefully.

## 6. Load generation

Use pgbench.

Scenarios:
- read-only
- write-heavy
- mixed

Primary autoscaling experiment should use the read path if adding replicas changes capacity.

## 7. Failover experiment

1. establish traffic
2. identify current primary
3. delete primary Pod
4. watch CNPG initiate failover
5. watch Strata status change
6. measure traffic recovery

Record:
- detection time
- failover interval
- request failures
- latency spike
- recovery time

CloudNativePG's automated-failover docs explicitly describe detecting failed primary health and initiating failover.

## 8. Rolling change experiment

Optional:
- modify resources
- observe rolling update behavior
- report state transition

Do not attempt major-version upgrade automation.
