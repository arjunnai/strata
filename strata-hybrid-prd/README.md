# Strata — Hybrid C++/Go Multi-Tenant Data & Inference Control Plane

Strata is a **4–6 week systems project** designed around one career goal:

> Build the resource-management/control-plane layer shared by modern managed database services and modern ML inference platforms, while improving real C++ systems-programming ability.

The project is intentionally hybrid:

- **C++20** — resource model, fair-sharing/admission, placement, autoscaling, simulator, policy server
- **Go** — Kubernetes CRDs/controllers, scheduler-framework adapter, CloudNativePG adapter, Modal adapter
- **Python** — only the small Modal/vLLM serving definition and optional benchmark scripts

## Core idea

```text
                           USER / CRDs
                               |
                               v
                   +------------------------+
                   | Go Integration Layer   |
                   |------------------------|
                   | Kubernetes controller  |
                   | scheduler adapter      |
                   | CloudNativePG adapter  |
                   | Modal adapter          |
                   +-----------+------------+
                               |
                              gRPC
                               |
                               v
                 +----------------------------+
                 |   C++ POLICY / CONTROL     |
                 |          ENGINE            |
                 |----------------------------|
                 | Resource accounting        |
                 | Tenant quota               |
                 | DRF-inspired admission     |
                 | Placement/filter/score     |
                 | Capacity planning          |
                 | Autoscaling policies       |
                 +------------+---------------+
                              |
                         deterministic
                         decisions
```

The **interesting algorithms live in C++**. Go exists where Kubernetes and cloud APIs make Go the natural integration language.

## Do not start with Kubernetes

Start here:

1. `00_START_HERE.md`
2. `02_CPP_CORE.md`
3. `03_SIMULATOR_ADMISSION.md`
4. `04_PLACEMENT_ENGINE.md`
5. `05_AUTOSCALER.md`
6. `06_GRPC_BOUNDARY.md`

Only after the C++ policy engine works should you move to:

7. `07_GO_CONTROL_PLANE.md`
8. `08_K8S_SCHEDULER_ADAPTER.md`
9. `09_POSTGRES_PROVIDER.md`
10. `10_MODAL_VLLM_PROVIDER.md`

Then finish with:

11. `11_OBSERVABILITY_RELIABILITY.md`
12. `12_EXPERIMENTS.md`
13. `13_TESTING_STRATEGY.md`
14. `14_AI_TUTOR_MODE.md`
15. `15_SIX_WEEK_PLAN.md`
16. `16_REPO_STRUCTURE.md`
17. `17_INTERVIEW_STORIES.md`

## What makes this project different

This is **not**:
- another toy database
- a generic Kubernetes project
- a set of Terraform manifests
- a vLLM wrapper
- a YAML-heavy DevOps portfolio project

It **is**:
- a scheduling/resource-management system
- a control-loop system
- a multi-tenant systems project
- a C++ algorithmic systems project
- a Kubernetes control-plane integration project
- a bridge from managed databases to inference infrastructure

## Official design references

- Kubernetes Scheduling Framework: https://kubernetes.io/docs/concepts/scheduling-eviction/scheduling-framework/
- Kubernetes client libraries: https://kubernetes.io/docs/reference/using-api/client-libraries/
- Kubebuilder good practices: https://book.kubebuilder.io/reference/good-practices
- gRPC C++: https://grpc.io/docs/languages/cpp/
- CloudNativePG: https://cloudnative-pg.io/
- Modal Go SDK: https://frontend.modal.com/docs/sdk/go/latest/Function
- vLLM distributed serving: https://docs.vllm.ai/en/latest/serving/data_parallel_deployment/
