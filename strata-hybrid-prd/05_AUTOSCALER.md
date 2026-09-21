# 05 — C++ Autoscaling Policy Engine

## Goal

Learn control loops by implementing autoscaling first as an offline/simulated policy, then connecting it to real DB and inference metrics.

## 1. Shared interface

Inputs:

```text
current_capacity
minimum_capacity
maximum_capacity
time-series metrics
SLO target
cooldown state
```

Output:

```text
desired_capacity
reason
signals
next_eligible_scale_time
```

## 2. Policy A — Static

Always:

```text
desired = current
```

Purpose:
- baseline

## 3. Policy B — Threshold

Example:

Scale up if:

```text
P99 latency > target
AND utilization > 70%
for 3 consecutive windows
```

Scale down if:

```text
utilization < 30%
AND latency safely below target
for 10 windows
```

## 4. Policy C — SLO-aware

Simple proportional idea:

```text
error_ratio =
    observed_slo / target_slo

raw_desired =
    ceil(current * error_ratio)
```

Then apply:
- min/max clamp
- maximum step size
- hysteresis
- cooldown

Do not turn this into a machine-learning project.

## 5. Why hysteresis exists

Without it:

```text
capacity:
2 -> 3 -> 2 -> 3 -> 2 -> 3
```

Build a synthetic workload that causes this.

Then fix it.

This is a learning objective, not just a feature.

## 6. Metric windows

Represent:
- timestamp
- metric value
- request rate
- utilization

Support:
- rolling mean
- rolling percentile if needed
- consecutive-window thresholds

Avoid complex time-series infrastructure in C++.

## 7. Database metric mapping

Potential signals:
- read P99 latency
- CPU
- active connections
- read request rate
- replica count

MVP action:
- change read-replica count

Do not horizontally scale the write primary.

## 8. Inference metric mapping

Signals:
- TTFT
- end-to-end latency
- concurrent requests
- queue depth or request backlog
- current container count

Action:
- update Modal autoscaler min/max/buffer settings

## 9. Required tests

- no scale during healthy state
- scale up on sustained breach
- do not scale on one noisy sample
- cooldown prevents repeated action
- never exceed max
- never go below min
- scale down slower than scale up
- invalid metric window produces safe decision

## 10. Experiment

Traffic trace:

```text
low
low
low
4x burst
4x burst
4x burst
low
low
```

Compare:
- static
- threshold
- SLO-aware

Measure:
- violation seconds
- capacity-time
- number of scale events
- recovery time
