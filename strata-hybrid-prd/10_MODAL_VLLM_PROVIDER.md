# 10 — Modal + vLLM Inference Provider

## Goal

Add real GPU inference as a second managed workload without becoming a kernel/compiler project.

## 1. Runtime

Use vLLM.

Official vLLM docs describe:
- data parallel deployment
- tensor parallel deployment
- multi-node serving
- external load balancing
- queue/KV-cache-aware routing opportunities

References:
- https://docs.vllm.ai/en/latest/serving/data_parallel_deployment/
- https://docs.vllm.ai/en/latest/serving/parallelism_scaling/

## 2. Provider

Use Modal for actual GPU capacity.

Modal's Go SDK provides runtime `UpdateAutoscaler` support for:
- min containers
- max containers
- buffer containers

Reference:
https://frontend.modal.com/docs/sdk/go/latest/Function

## 3. Language split

### Python

Only:
- Modal app definition
- vLLM server bootstrap

### Go

- locate named Modal function
- read endpoint/config if needed
- call autoscaler update
- health probe
- collect provider state

### C++

- decide desired capacity

## 4. MVP model

Pick a model that:
- runs comfortably on affordable Modal GPU
- has stable vLLM support
- lets you repeat experiments cheaply

Do not choose a huge model simply because credits exist.

Repeated experiments matter more than model size.

## 5. Traffic generator

Create fixed workload classes:

### Short prompt

Useful for throughput.

### Long prompt

Useful for TTFT/prefill pressure.

### Burst

Arrival rate jumps by 4–5x.

### Mixed tenant

Different logical tenants generate traffic.

## 6. Required metrics

Collect:
- TTFT
- end-to-end latency
- error rate
- requests/sec
- concurrent requests
- current capacity
- scale events

Optional:
- tokens/sec
- prompt/output token counts
- estimated spend

## 7. Autoscaling experiment

C++ policy receives metric window.

Example:

```text
target TTFT = 500 ms
observed P99 TTFT = 930 ms
current capacity = 2
queue growing
```

Returns:

```text
desired capacity = 4
reason = SustainedSLOBreach
```

Go calls Modal.

## 8. Important honesty boundary

Strata does **not** choose Modal physical GPU nodes.

Do not describe the project as a GPU cluster scheduler for Modal.

You are implementing:
- platform admission
- capacity decisions
- autoscaling
- workload policy

The Kubernetes simulator/scheduler side demonstrates placement logic.

## 9. Stretch: request routing

Only after core project is complete.

Possible policies:
- round robin
- least queue
- power-of-two
- prefix-aware affinity

vLLM docs note that external load balancing can use real-time telemetry and that KV-cache state matters for routing.

Do not let this stretch goal delay the finished project.
