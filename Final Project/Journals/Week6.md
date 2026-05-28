# Journal Entry Week 6

## 1. What I built this week
This week focused on completing and refining the visualization layer of the particle system while pushing performance closer to real-time limits at large scale. A major improvement was made to the NumPy-based implementation by switching the rendering operations from per-particle drawing calls (using a for-loop) to a fully vectorized pixel buffer approach using pygame.surfarray. This majorly reduced Python-level overhead and allowed the system to handle higher particle counts with stable frame rates. We're now comfortably rendering one-million particles at 30 FPS at the low end.

Also, the linked list visualization was successfully implemented and tested. While it remains substantially slower than the vectorized version, it can still render approximately 50,000 particles in real time, providing a clear comparison between pointer-based traversal and contiguous memory approaches. This will be it for the visualization aspect of the project.

## 2. Code highlight (vectorized rendering optimization)

```py
# Convert coordinates to integer arrays
x_int = ps.x.astype(np.int32)
y_int = ps.y.astype(np.int32)

# Keep only visible particles
visible = (
    (x_int >= 0) &
    (x_int < WIDTH) &
    (y_int >= 0) &
    (y_int < HEIGHT)
)

x_int = x_int[visible]
y_int = y_int[visible]

# Access screen pixel buffer
pixels = pygame.surfarray.pixels2d(screen)

# Draw all particles at once
pixels[x_int, y_int] = 0xFFFFFF

# Release pixel buffer
del pixels
```
This optimization replaces Python-level per-particle rendering with a vectorized pipeline that operates directly on the frame buffer. Particle positions are filtered using NumPy 'boolean masking', and valid coordinates are written in bulk to the screen memory. The boolean masking basically applies vectors (arrays) of true/false statements to vectors of particles rather than checking them sequentially (one by one with a loop), preventing undue resources from being allocated to off-screen particles. This reduces per-frame Python overhead and shifts the bottleneck toward memory bandwidth rather than CPU-bound iteration.

## 3. Complexity checkpoint
All three structures still exhibit linear time complexity with respect to particle count:

O(n)

However, performance differs significantly due to implementation-level constant factors:

- Python list: linear iteration with object-level updates and Python-managed - filtering overhead
- Linked list: linear traversal with pointer chasing and per-node deletion cost
- NumPy vectorized system: linear operations executed in compiled C code over contiguous memory, with additional gains from batch pixel writes during rendering

For the final write-up, specific complexity formulas for each structure will be outlined detailing the differences in linear growth of each.

## 4. What broke / what was hard
The primary challenge this week was optimizing the visualization layer to handle large particle counts without degrading frame rate. Initial rendering approaches using per-particle drawing calls in Pygame were too slow at scale. 

The linked list visualization also exposed inherent performance limitations of pointer-based traversal under real-time rendering constraints. While functional, it highlights the trade-off between structural flexibility and cache-efficient memory access. I don't think this visualization will make it to 1 million by presentation day, but it suffices to show the tradeoffs in structure selection, and how selection can cut off downstream optimization routes (like was done with boolean masking for VectArray in PyGame).

## 5. Plan for next week
The final week will focus on consolidating results into the final report and presentation. This includes formal benchmarking analysis using PPS and FPS data, structured comparison of all three implementations (basically complete), and a detailed complexity discussion emphasizing not only Big-O behavior but also constant-factor differences driven by memory layout and vectorization capability. The project will conclude with a reflection on how data structure selection influences both algorithmic performance and the feasibility of downstream optimizations such as hardware-accelerated computation and batch processing, maybe touching on the importance of contiguous memory operations in the face of ever increasing data and marginal increases in compute as time goes on. The close-to-hardware solutions of the 'old days' may be in vogue once again, by necessity.