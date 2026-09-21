# 14 — AI Tutor Mode Contract

## Purpose

Use AI to accelerate understanding without outsourcing the implementation.

## Default rule

> Do not ask AI for implementation code until you have written an attempt.

## Good AI requests

### Concept teaching

```text
Explain dominant resource fairness using this example.
Do not give me code.
```

### Design grilling

```text
Here is my ResourceVector API.
Ask me questions that expose bad invariants.
Do not redesign it for me yet.
```

### Progressive debugging

```text
My admission test fails.
Give me one hint at a time.
Do not show the fix.
```

### Code review

```text
Review this implementation for correctness, ownership, edge cases,
and C++ style. Do not rewrite it.
```

### Test review

```text
Here are my tests.
What failure classes am I missing?
Do not write the tests.
```

## Bad AI requests for this project

Avoid:

```text
Implement DRF in C++.
Build the Kubernetes operator.
Write the scheduler plugin.
Generate the full repo.
Fix all these errors and return working code.
```

That defeats the point.

## Escalation ladder

When stuck:

### Level 1
Ask for a conceptual explanation.

### Level 2
Ask for one hint.

### Level 3
Show your code and ask where the bug category is.

### Level 4
Ask for pseudocode.

### Level 5
Only if still blocked, ask for a minimal isolated example.

Do not jump directly to level 5.

## Personal checkpoint

Before accepting AI advice, answer:

1. Can I explain why this change works?
2. Could I reproduce it tomorrow without the chat?
3. Does this preserve my design invariants?
4. Did I add a test for the failure?

If no, do not paste it.

## Interview preparation benefit

Keep a file:

```text
docs/decisions.md
```

For every meaningful design change record:

```text
Problem
Options considered
Decision
Why
Failure mode
What changed after testing
```

This gives you authentic interview stories.
