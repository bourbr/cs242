# Week 4 Journal Entry

## 1. What I built this week

This week I expanded and optimized all three particle system implementations (Python list, linked list, and NumPy vectorized array). The main focus was improving performance and adding a first working visualization layer using Pygame. Also benchmarked at 1 million for all three structures.

The benchmarking framework was refined to compare systems across multiple input sizes (10K–1M particles), and an initial real-time visualization was implemented using the vectorized NumPy system. The visualization confirms that the simulation behaves correctly under load and that decay reduces active particle count over time.

## 2. Code highlight
Linked list optimization (structural and memory efficiency improvement)

```py
class Node:
    __slots__ = ('particle', 'next')
    
    def __init__(self, particle):
        self.particle = particle
        self.next = None
```
I learned that __slots__ improves efficiency by reducing dynamic attribute dictionary allocation, per object. It's a relatively minor improvement but one of the few I can implement for the linked list structure.

```py
def spawn(self, n):
    self.x = np.random.random(n).astype(np.float32)
    self.y = np.random.random(n).astype(np.float32)

    self.vx = np.random.uniform(-1, 1, n).astype(np.float32)
    self.vy = np.random.uniform(-1, 1, n).astype(np.float32)

    self.life = np.random.randint(50, 100, n).astype(np.int32)
```
This change allows the vector array to allocate memory once, rather than appending and reallocating during runtime, by generating arrays directly

## 3. Complexity Checkpoint

All structures are still O(n), which for the scope of this project and the structures selected is expected.

## 4. What broke / what was hard

The most difficult part this week was integrating Pygame for visualization. While not conceptually complex, it was unfamiliar, and getting a stable render loop with particle updates, FPS control, and overlay text required some trial and error.

The visualization chugs along at around 18 fps at 50k particles, using the most efficient structure I have implemented (vector array).

Considering maybe implementing a visualization for each data structure, but if vector arrays are performing only adequately at 50k particles these would have to be pretty lightweight, especially for the native python array; not sure how instructive that would be, but could be worthwhile for a sense of completion presentation-wise.

## 5. Plan for next week

- Improve rendering performance in Pygame (reduce draw overhead at high particle counts)
- Explore lightweight visualization optimizations (e.g., batching or alternative drawing strategies)
- Begin write-up and march toward completion.


