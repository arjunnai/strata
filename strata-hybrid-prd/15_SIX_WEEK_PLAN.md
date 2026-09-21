# 15 — Six-Week Build Plan

## General rule

The project should become resume-usable by the end of **Week 4**.

Weeks 5–6 add real inference and polish.

---

# Week 1 — C++ resource-management foundation

## Day 1

Learn/design:
- resource vectors
- invariants

Build:
- C++ project skeleton
- ResourceVector
- Node
- Workload

Tests:
- fit
- allocation/release
- invalid values

## Day 2

Build:
- Tenant
- quota accounting
- ClusterSnapshot

Tests:
- hard quota
- duplicate accounting
- release

## Day 3

Build:
- FIFO admission
- simulator shell

Run hand-written scenarios.

## Day 4

Learn DRF.

Calculate examples manually.

Build:
- dominant share
- DRF-inspired admission

## Day 5

Build:
- trace runner
- fairness metrics
- tests

### Week 1 exit criteria

```text
./strata-sim scenario
```

can produce repeatable fair-admission decisions.

---

# Week 2 — Placement + autoscaling

## Day 6

Build:
- placement Filter
- feasibility reasons

## Day 7

Build:
- Score
- fit
- fragmentation

## Day 8

Build:
- spread/pack
- multi-replica greedy placement

## Day 9

Build:
- static + threshold autoscaler

## Day 10

Build:
- SLO-aware policy
- hysteresis/cooldown

### Week 2 exit criteria

Standalone C++ system can:
- admit
- place
- scale

without Kubernetes.

---

# Week 3 — gRPC + Kubernetes fundamentals

## Day 11

Learn gRPC C++ basics.

Define:
- proto
- EvaluateAdmission

Get C++ server + small client working.

## Day 12

Add:
- placement RPC
- scaling RPC
- deadlines/errors

## Day 13

Learn Kubebuilder.

Build throwaway Echo operator.

## Day 14

Break Echo:
- delete child
- restart controller
- update desired count

Understand convergence.

## Day 15

Create:
- Tenant CRD
- StrataWorkload CRD
- Go -> C++ admission integration

### Week 3 exit criteria

A Kubernetes resource causes a real C++ policy decision.

---

# Week 4 — Scheduler + PostgreSQL

## Day 16

Build scheduler plugin Filter adapter.

## Day 17

Build Score adapter.

Test with synthetic resources.

## Day 18

Install CloudNativePG.

Write provider Ensure/Observe.

## Day 19

Add:
- deletion/finalizer
- health/status translation
- pgbench

## Day 20

Run:
- Postgres primary failure
- controller restart test

### Week 4 exit criteria

You now have a legitimate:
- distributed systems
- managed DB
- Kubernetes control plane
- C++ resource manager

project.

Start putting it on resume if needed.

---

# Week 5 — Autoscaling + Modal/vLLM

## Day 21

Wire metrics into scaling decision.

Run DB burst experiment.

## Day 22

Deploy minimal vLLM app on Modal.

Measure TTFT manually.

## Day 23

Build Go Modal adapter.

Verify `UpdateAutoscaler`.

## Day 24

Connect inference metrics -> C++ scale policy -> Modal.

## Day 25

Run inference burst experiments.

### Week 5 exit criteria

Same C++ autoscaler operates on:
- DB SLO
- inference SLO

---

# Week 6 — Evaluation and polish

## Day 26

Run fairness experiments.

## Day 27

Run placement experiments.

## Day 28

Run chaos/failure experiments.

## Day 29

Generate final graphs.

Write:
- architecture
- tradeoffs
- benchmark findings

## Day 30

Record demo.

Clean:
- README
- setup
- examples
- issues
- resume bullets

---

# Cut list

If behind, cut in this order:

1. prefix-aware routing
2. preemption
3. predictive scaling
4. cost accounting
5. fancy CLI
6. extra scheduler hooks

Never cut:
- C++ simulator
- fair admission
- placement
- autoscaling
- gRPC
- controller reconciliation
- Postgres
- Modal/vLLM
- experiments
