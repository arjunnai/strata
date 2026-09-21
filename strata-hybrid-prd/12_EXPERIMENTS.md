# 12 — Experiments and Evaluation

## Goal

Make the project evidence-driven.

The project is much stronger if you can say:

> Policy X reduced P95 queue time by Y while increasing idle capacity by Z.

rather than:

> I implemented autoscaling.

## Experiment 1 — Fair admission

Policies:
- FIFO
- round robin
- DRF-inspired

Workload:
- CPU-heavy tenant
- memory-heavy tenant
- GPU-heavy tenant

Measure:
- mean queue delay
- P95 queue delay
- dominant share
- utilization
- starvation count

## Experiment 2 — Placement fragmentation

Policies:
- first fit
- best fit
- Strata weighted score

Workloads:
- small CPU
- large CPU/memory
- 1/2/4 synthetic GPU

Measure:
- successful placements
- stranded resources
- utilization
- pending count

## Experiment 3 — DB autoscaling

Policies:
- static
- threshold
- SLO-aware

Traffic:
```text
baseline
baseline
4x burst
4x burst
baseline
```

Measure:
- P99
- throughput
- replicas
- SLO violation seconds
- replica-minutes

## Experiment 4 — Inference autoscaling

Same policy comparison.

Measure:
- TTFT P50/P99
- E2E P99
- concurrency
- capacity
- violation seconds
- container/GPU time proxy

## Experiment 5 — Control-plane failure

Kill:
- C++ policy engine
- Go controller

Measure:
- recovery time
- duplicate actions
- workload impact

## Experiment 6 — Postgres failover

Kill primary.

Measure:
- detection
- failover duration
- failed requests
- P99 spike
- recovery

## Reproducibility

Target commands:

```text
make experiment-fairness
make experiment-placement
make experiment-db-scale
make experiment-inference-scale
make experiment-controller-chaos
make experiment-db-failover
```

## Results directory

```text
results/
  fairness/
  placement/
  db-scale/
  inference-scale/
  chaos/
```

Store:
- config
- raw CSV/JSON
- logs
- generated plots

## Required final graphs

At least:
1. tenant dominant share over time
2. placement utilization/fragmentation
3. DB P99 + replica count over time
4. inference TTFT + capacity over time
5. failure/recovery timeline

## Research-style discipline

Do not cherry-pick.

If Strata policy loses on one metric:
- report it
- explain tradeoff
- refine if justified

That is stronger systems work than pretending every policy wins.
