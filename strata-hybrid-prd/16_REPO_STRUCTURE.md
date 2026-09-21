# 16 — Repository Structure

```text
strata/
├── README.md
├── CMakeLists.txt
├── Makefile
│
├── proto/
│   └── policy.proto
│
├── cpp/
│   ├── include/strata/
│   │   ├── resource_vector.h
│   │   ├── tenant.h
│   │   ├── workload.h
│   │   ├── node.h
│   │   ├── cluster_snapshot.h
│   │   ├── admission.h
│   │   ├── placement.h
│   │   └── autoscaler.h
│   │
│   ├── src/
│   │   ├── resource_vector.cc
│   │   ├── admission/
│   │   ├── placement/
│   │   ├── autoscaler/
│   │   └── grpc/
│   │
│   ├── simulator/
│   │   └── main.cc
│   │
│   └── tests/
│
├── go/
│   ├── api/
│   │   └── v1alpha1/
│   ├── cmd/
│   │   ├── controller/
│   │   └── scheduler/
│   ├── internal/
│   │   ├── controller/
│   │   ├── policyclient/
│   │   ├── scheduler/
│   │   └── provider/
│   │       ├── postgres/
│   │       └── modal/
│   └── go.mod
│
├── inference/
│   └── modal_vllm.py
│
├── deploy/
│   ├── crds/
│   ├── controller/
│   ├── scheduler/
│   ├── prometheus/
│   └── grafana/
│
├── scenarios/
│   ├── fairness/
│   ├── fragmentation/
│   └── autoscaling/
│
├── experiments/
│   ├── scripts/
│   ├── configs/
│   └── results/
│
└── docs/
    ├── architecture.md
    ├── decisions.md
    ├── benchmark-results.md
    └── devlog/
```

## Dependency direction

Very important:

```text
C++ core
   ^
   |
C++ gRPC service

Go controller ---> generated proto client

providers do not import policy implementation
```

The C++ policy library should be usable by:
- simulator
- unit tests
- gRPC service

without Kubernetes.

## Build targets

Desired eventual commands:

```text
make cpp-test
make cpp-sim
make policy-server
make go-test
make controller
make scheduler
make local-cluster
make postgres-demo
make inference-demo
make experiment-all
```

Do not implement the Makefile all at once.

Add targets as the project grows.
