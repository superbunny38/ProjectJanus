# Project Janus: The Legacy-Modern Bridge

> The Mission: Build a high-performance, thread-safe search indexer in Modern C++20 that interfaces safely with a fragile, non-thread-safe Legacy C++98 data feed.

## 📖 Overview

**Project Janus** is a simulation of a common high-stakes scenario in large-scale software engineering: **Modernization without Rewrite.**

The goal is to attach a modern ranking and indexing engine to an ancient data ingestion system ("The Legacy Feed"). The legacy system is slow, uses raw pointers, manual memory management, and is not thread-safe. My system acts as a bridge—consuming unsafe data, sanitizing it, and indexing it in parallel using modern C++ design patterns.

## ⚠️ The Constraint

To replicate a real-world production environment, I adhere to one strict rule:

**I am strictly forbidden from modifying `LegacyDataFeed.h`.**

This file represents a "Read-Only" library provided by another team. I cannot fix their memory leaks or add locks to their code. I must handle their "bad" code externally via the Adapter Pattern and rigorous RAII principles to prevent it from infecting the modern codebase.

## 🛠 Tech Stack & Concepts

- **Language**: C++20

- **Key Patterns**: Adapter Pattern, Producer-Consumer, Sharding

- **Memory Management**: RAII, `std::unique_ptr`, `std::span`, Zero-Copy semantics

- **Concurrency**: `std::jthread`, `std::condition_variable`, Fine-grained Locking

- **Optimization**: Cache Locality (Data Oriented Design), Custom Hash Maps

## 🏗 Architecture Roadmap

### Phase 1: The Adapter Layer ("The Sanitizer")

**Status**: 🚧 In Progress

- **Challenge**: The Legacy Feed emits raw pointers (LegacyDoc*) allocated with new. If not handled, these leak memory.

- **Solution**: A ModernAdapter class that wraps the legacy callback. It immediately converts raw pointers into modern, managed objects (std::unique_ptr or value types) and ensures the legacy memory is freed before leaving the scope.

### Phase 2: The "Kafka-Lite" Broker

**Status**: 📅 Planned

- **Challenge**: The Legacy Feed blocks and is slow (simulated network/disk IO). The Indexer is fast.

- **Solution**: A thread-safe generic Queue using std::condition_variable. This decouples the Producer (Legacy Feed) from the Consumer (Indexer), allowing them to operate at different speeds without data loss.

### Phase 3: The Sharded Inverted Index

**Status**: 📅 Planned

- **Challenge**: A single `std::map` with a Global Lock kills parallelism.

- **Solution**: Sharding. The index is split into 16 smaller hashmaps based on keyword hashing. This allows multiple threads to write to the index simultaneously with minimal lock contention.

### Phase 4: Query Engine & Ranking

**Status**: 📅 Planned

- **Challenge**: Retrieving relevant results quickly.

- **Solution**: A ranking function based on Term Frequency (TF) to return the top 10 results for a given query.

### Phase 5: Optimization 

**Status**: 📅 Planned

- **Challenge**: `std::map` causes cache misses due to pointer chasing.

- **Solution**: Profiling the code and replacing node-based containers with contiguous memory containers (`std::vector` + `std::binary_search`) to improve CPU cache locality.

## 🚀 Getting Started

**Prerequisites**

- C++ Compiler supporting C++20 (GCC 10+, Clang 10+, MSVC 19.28+)

- CMake

**Build Instructions**

```
mkdir build
cd build
cmake ..
make
./ProjectJanus
```

## 📂 Project Structure
```
ProjectJanus/
├── src/
│   ├── main.cpp            # Entry point
│   ├── ModernAdapter.cpp   # The sanitizer logic
│   ├── Indexer.cpp         # The sharded index logic
│   └── ...
├── include/
│   ├── LegacyDataFeed.h    # ⛔ READ-ONLY LEGACY CODE
│   ├── ModernAdapter.h
│   └── ...
├── CMakeLists.txt
└── README.md
```

## ✅ Progress Log

- [ ] Day 46-47: Adapter Pattern & RAII Compliance

- [ ] Day 48-49: Thread-Safe Queue Implementation

- [ ] Day 50-52: Sharded Inverted Index

- [ ] Day 53-54: Query Engine & Ranking Logic

- [ ] Day 55: Optimization (Vector vs Map)