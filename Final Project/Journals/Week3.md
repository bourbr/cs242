# Week 3 Journal Entry

## 1. What I built this week

This week I implemented a formal benchmarking framework for the particle system and extended the project toward larger-scale performance testing (100K–500K particles per system). The main improvement was replacing ad-hoc timing blocks with a reusable benchmark function that evaluates each implementation (array-based, linked list, and NumPy-based) under identical conditions.

Each system is now tested using fresh state per run, meaning every benchmark begins with a newly instantiated particle system and a clean spawn of N particles. This ensures results are consistent and not influenced by leftover state or previous updates. The system now produces structured timing results across multiple input sizes, forming the basis for later complexity analysis.

## 2. Code highlight

```cpp
def benchmark_system(system_class, name, N, steps=60):
    system = system_class()   # always fresh instance
    system.spawn(N)

    start = time.perf_counter()

    for _ in range(steps):
        system.update()

    end = time.perf_counter()

    return {
        "name": name,
        "N": N,
        "time": end - start,
        "remaining": system.count() if hasattr(system, "count") else len(system.particles)
    }
```

This function is the core of the benchmarking framework. It ensures each system is tested from a clean initial state, which is important for fairness and repeatability across different data structures. It also standardizes timing and output formatting so results can be directly compared across implementations and input sizes.

## 3. Complexity checkpoint

- Array (Python list): O(n) update + O(n) rebuild each frame due to filtering live particles
- Linked list: O(n) traversal with O(1) deletions, but higher constant overhead due to pointer chasing
- NumPy system: O(n) vectorized operations in compiled C, with additional O(n) masking for deletion

The theoretical big O complexity of all three update loops is still linear in terms of particle count, but their constant factors differ significantly. At 500K elements, NumPy shows a large performance advantage due to the vectorized execution, while the array and linked list implementations remain close in performance, with the linked list slightly faster in some runs due to avoiding full array rebuilds. I'm not sure I'll be keeping the Python array structure through to the end of the project, but for now it's interesting to compare the other two structures against it. 

## 4. What broke / what was hard

One challenge was ensuring fair benchmarking across all implementations. Early versions reused system state between runs, which could distort results, so each benchmark now explicitly creates a fresh instance of the system. 

## 5. Plan for next week

The next step is to scale toward 1 million particles and begin integrating visualization using a lightweight graphics library (likely Pygame). The goal is to visually represent the particle system in real time while continuing to measure performance under load.

This will also involve refining the benchmarking results and preparing them for formal analysis, including structured tables for 10K, 100K, and 1M particle tests. The NumPy implementation may also be further optimized to reduce overhead from repeated array resizing and masking operations.