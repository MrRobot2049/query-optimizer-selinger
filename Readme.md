# 📘 Query Optimizer Simulator (C++)

## Overview

This project implements a **cost-based SQL query optimizer** inspired by **System R (Selinger) optimization**.
The optimizer estimates join costs and cardinalities, enumerates valid join orders using **dynamic programming**, and selects the **lowest-cost execution plan**.

Unlike typical DBMS projects that focus on SQL parsing or CRUD operations, this project focuses on **how real databases decide *which* query plan to execute**.

---

## Key Features

* **Selinger-style Dynamic Programming**

  * Enumerates join orders using subset-based DP
  * Stores one optimal plan per subset of relations
  * Restricts search to left-deep join trees (as in real systems)

* **Cost-Based Optimization**

  * I/O-aware cost model
  * Hash join cost estimation
  * Comparison against naive left-to-right join execution

* **Cardinality Estimation with Statistics Propagation**

  * Attribute-level distinct counts
  * Correct equi-join cardinality estimation
    |R ⋈ S| = (|R| × |S|) / max(V(R.a), V(S.b))
  * Statistics propagated across intermediate join results

* **Predicate-Aware Join Enumeration**

  * Invalid joins are pruned early
  * Join predicates treated symmetrically and safely

* **Query Plan Visualization**

  * ASCII visualization of optimized join trees
  * Makes execution structure easy to understand

---

## Architecture

```
include/
 ├── optimizer.h     // Optimizer API
 ├── plan.h          // Plan representation + statistics
 ├── stats.h         // Relation metadata
 ├── predicate.h     // Join predicates
 └── cost.h          // Cost & cardinality estimation

src/
 ├── optimizer.cpp   // Selinger DP implementation
 ├── cost.cpp        // Cost + cardinality logic
 ├── main.cpp        // Driver program
 └── ...
```

---

## How the Optimizer Works

### 1. Base Scan Plans

Each relation starts with a sequential scan plan:

```
best({R}) = Scan(R)
best({S}) = Scan(S)
```

### 2. Dynamic Programming (Selinger)

For any subset of relations `S`:

best(S) = min over r in S of(best(S \ {r}) ⋈ r)

Only the cheapest plan for each subset is retained.

### 3. Cost & Cardinality Estimation

* Uses attribute-level statistics
* Handles predicate direction robustly
* Avoids zero or invalid cardinalities

### 4. Plan Selection

The optimizer compares:

* **Naive plan** (left-to-right joins)
* **Optimized plan** (cost-based join reordering)

---

## Example Output

### Naive Plan

```
((R ⨝ S) ⨝ T)
Cost: 8,200,000 I/Os
Rows: 20,000,000
```

### Optimized Plan

```
((T ⨝ S) ⨝ R)
Cost: 1,900,000 I/Os
Rows: 5,000,000
```

### Optimized Plan (Tree View)

```
        R
    ⨝
        S
    ⨝
        T
```

---

## Build & Run

### Requirements

* C++17 compatible compiler (`g++` / `clang++`)
* macOS / Linux

### Compile

```bash
make clean
make
```

### Run

```bash
./optimizer
```

---

## Assumptions & Simplifications

* Uniform data distribution
* Attribute independence
* Equi-joins only
* No indexes (sequential scans assumed)
* Cost measured in logical I/O units

These assumptions mirror early System R–style optimizers and are explicitly documented.

---

## Future Extensions

* Index-aware cost model
* Bushy join trees
* SQL parser frontend
* Histogram-based statistics
* Graphviz plan visualization







