# 00 — Start Here: How to Build Strata Without Outsourcing the Learning

## Rule zero

Do **not** begin by asking an AI system to generate Strata.

The project is useful only if you can independently reason about:
- resource allocation
- fairness
- placement
- control loops
- reconciliation
- failure cases

## Development order

Build Strata in this order:

```text
C++ domain model
      |
      v
standalone simulator
      |
      v
fair admission
      |
      v
placement engine
      |
      v
autoscaler
      |
      v
gRPC boundary
      |
      v
small Go reconciliation exercise
      |
      v
real Strata Kubernetes controller
      |
      v
scheduler integration
      |
      v
Postgres
      |
      v
Modal/vLLM
      |
      v
experiments
```

## First milestone: no cloud, no Kubernetes

Your first executable should be:

```bash
./strata-sim scenario.json
```

It should accept:
- nodes
- tenants
- current allocations
- queued workloads

and output decisions such as:

```text
Tenant payments dominant share: 0.42
Tenant search dominant share:   0.25

Next admitted workload:
  search/query-worker-7

Placement:
  node-3

Reason:
  tenant has lowest dominant share
  node-3 minimizes resource fragmentation
```

If this does not work, you are not ready to integrate Kubernetes.

## Learning loop for every subsystem

Use this exact cycle:

### 1. Learn

Read one focused source.

Examples:
- DRF paper/summary
- Kubernetes scheduling framework docs
- Kubebuilder reconciliation docs
- gRPC C++ tutorial

### 2. Explain it yourself

Before coding, write 5–10 sentences answering:
- what problem does this solve?
- what state does it need?
- what invariants should hold?
- how can it fail?

### 3. Implement a small version

Do not implement the final abstraction first.

Example:
- first only CPU
- then CPU + memory
- then GPU
- then weighted tenants

### 4. Break it

Write adversarial cases.

### 5. Review

Only after your implementation exists:
- ask AI/code review
- inspect incorrect assumptions
- improve it yourself

## Daily development journal

Maintain:

```text
docs/devlog/
  day01.md
  day02.md
  ...
```

Each day record:
- what I built
- what I misunderstood
- bug that surprised me
- one design decision
- what I would explain in an interview

This makes interview preparation much easier later.

## Definition of "I understand it"

You understand a subsystem when you can answer:

1. Why does it exist?
2. What invariant does it enforce?
3. What is the algorithm?
4. What is its complexity?
5. What failure mode scared you?
6. What tradeoff did you choose?
7. What would break at 100x scale?

Do not move on merely because the code compiles.
