# 17 — Interview Stories to Intentionally Create

## Goal

A strong project gives you debugging/design stories, not just features.

Do not manufacture these. Use this document to notice them when they occur.

## Story 1 — Fairness vs utilization

Ideal experience:
- DRF policy behaves fairly
- large head-of-line request reduces utilization
- you change to work-conserving behavior
- quantify tradeoff

Topics:
- scheduling
- fairness
- starvation
- design tradeoff

## Story 2 — Resource fragmentation

Ideal experience:
- naive first-fit causes 4-GPU workload to remain pending
- capacity exists in aggregate but is fragmented
- score function improves placement

Topics:
- bin packing
- resource scheduling
- fragmentation

## Story 3 — Autoscaler instability

Ideal experience:
- threshold scaler oscillates
- you identify feedback delay/noise
- add hysteresis/cooldown
- measure improvement

Topics:
- control loops
- distributed systems
- reliability

## Story 4 — Idempotency bug

Ideal experience:
- provider creation succeeds
- controller misses/loses status update
- retry duplicates or nearly duplicates resource
- fix reconciliation around observed state

Topics:
- control planes
- failure semantics
- idempotency

## Story 5 — DB failover

Ideal experience:
- kill primary
- control plane observes failover
- transient metrics mislead autoscaler
- you prevent inappropriate scaling during unstable state

Topics:
- stateful services
- health
- control-plane coordination

## Story 6 — Inference burst

Ideal experience:
- TTFT spikes under burst
- capacity expansion has cold-start delay
- SLO-aware scaler reacts
- overprovisioning vs latency tradeoff

Topics:
- ML systems
- autoscaling
- capacity

## Story 7 — C++/Go boundary

Ideal experience:
- initial proto leaks Kubernetes details
- difficult to test
- redesign into infrastructure-neutral policy objects

Topics:
- API design
- language boundaries
- architecture

## Final interview answer structure

For any story:

```text
Context
Constraint
Naive design
Failure observed
Root cause
Change made
Measurement
Tradeoff
What I'd do at 100x scale
```

Keep benchmark numbers real.
