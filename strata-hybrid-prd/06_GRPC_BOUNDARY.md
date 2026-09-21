# 06 — gRPC Boundary Between Go and C++

## Goal

Expose the C++ policy engine through a small, explicit contract.

Official C++ gRPC docs:
https://grpc.io/docs/languages/cpp/

## 1. Build one RPC first

Start with:

```text
EvaluateAdmission
```

Only after that works add:
- EvaluatePlacement
- EvaluateScaling

## 2. Design rule

The `.proto` file contains **Strata concepts**, not Kubernetes concepts.

Bad:
```text
Pod
NodeStatus
ObjectMeta
```

Good:
```text
ResourceVector
TenantUsage
WorkloadRequest
NodeSnapshot
```

## 3. Suggested RPCs

Conceptually:

```proto
service PolicyEngine {
  rpc EvaluateAdmission(AdmissionRequest)
      returns (AdmissionDecision);

  rpc EvaluatePlacement(PlacementRequest)
      returns (PlacementDecision);

  rpc EvaluateScaling(ScalingRequest)
      returns (ScalingDecision);
}
```

Do not add streaming RPCs unless a real need appears.

## 4. Admission request

Needs:
- tenant ID
- tenant weight
- tenant quota
- tenant current usage
- cluster capacity
- current allocations
- queued workload

Response:
- admitted?
- reason
- dominant share
- debug fields

## 5. Placement request

Needs:
- workload
- candidate nodes
- existing replica locations
- placement preference

Response:
- selected node
- node scores
- rejected node reasons

## 6. Scaling request

Needs:
- workload/service identifier
- current capacity
- min/max
- SLO
- metric window
- current timestamp/cooldown info

Response:
- desired capacity
- reason
- signals

## 7. Versioning

For MVP:
- one proto package version, e.g. `strata.policy.v1`

Do not implement elaborate compatibility machinery.

## 8. Timeouts

Go client must set deadlines.

Policy calls should be fast.

Example target:
- admission < 50 ms
- placement < 100 ms for small demo clusters

## 9. Failure behavior

If C++ policy service is down:
- existing workloads continue running
- new policy-dependent action stays pending
- Go controller records condition
- retry with backoff

Do not fall back to an unsafe guessed decision.

## 10. Tests

- Go integration test against C++ service
- timeout path
- malformed request
- unavailable service
- deterministic repeated request
- proto serialization round trip
