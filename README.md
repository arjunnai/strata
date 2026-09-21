# Strata

A multi-tenant control plane and resource manager for managed database
and inference workloads.

## Status

Early development.

## Build

```bash
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
