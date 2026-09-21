# Day 1

## Goal

Set up the C++ development environment for Strata.

## Decisions

- C++20
- CMake
- GoogleTest
- clang-format
- strata_core is a library shared by simulator, tests, and later gRPC server

## Things I learned

### What is a CMake target?
A target is a named build unit managed by CMake such as a library exe or a test file, could also contain source files include paths compiler settings and deps.

### Why is strata_core a library instead of an executable?
strata_core contains reusable project logic

### What is FetchContent doing?
downloads googletest version v1.17.0 during cmake config and makes targets available to the project.

### What is the difference between the source tree and build tree?
source tree -> file we write and maintain

build tree -> whatever gets generated or compiled

