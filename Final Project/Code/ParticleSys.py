import random
import time
import numpy as np
import csv
import pandas as pd
import matplotlib.pyplot as plt

# ---------------------------
# Particle Class Definition
# ---------------------------
class Particle:
    __slots__ = ('x', 'y', 'vx', 'vy', 'life')

    def __init__(self, x, y, vx, vy, life):
        self.x = x
        self.y = y
        self.vx = vx
        self.vy = vy
        self.life = life
        
    def update(self): # Used by Native Python Array-based system
        self.x += self.vx
        self.y += self.vy
        self.life -= 1

# ---------------------------
# Particle System Class Numpy-based
# ---------------------------
class ParticleSystemNumPy:
    def __init__(self):
        self.x = np.array([], dtype=np.float32)
        self.y = np.array([], dtype=np.float32)
        self.vx = np.array([], dtype=np.float32)
        self.vy = np.array([], dtype=np.float32)
        self.life = np.array([], dtype=np.int32)
    
    def spawn(self, n):
        self.x = np.random.random(n).astype(np.float32)
        self.y = np.random.random(n).astype(np.float32)

        self.vx = np.random.uniform(-1, 1, n).astype(np.float32)
        self.vy = np.random.uniform(-1, 1, n).astype(np.float32)

        self.life = np.random.randint(50, 100, n).astype(np.int32)

    def update(self):
        # VECTOR UPDATE 
        self.x += self.vx
        self.y += self.vy
        self.life -= 1

        # VECTOR DELETE 
        alive_mask = self.life > 0

        self.x = self.x[alive_mask]
        self.y = self.y[alive_mask]
        self.vx = self.vx[alive_mask]
        self.vy = self.vy[alive_mask]
        self.life = self.life[alive_mask]

    def count(self):
        return len(self.life)

# ---------------------------
# Particle Node Class (Linked List)
# ---------------------------
class Node:
    __slots__ = ('particle', 'next')

    def __init__(self, particle):
        self.particle = particle
        self.next = None

# ---------------------------
# Particle Linked List Class
# ---------------------------
class ParticleLinkedList:
    def __init__(self):
        self.head = None
        self.tail = None
        self.size = 0

    def append(self, particle):
        new_node = Node(particle)

        if not self.head:
            self.head = self.tail = new_node
        else:
            self.tail.next = new_node
            self.tail = new_node

        self.size += 1

    def update(self):
        current = self.head
        prev = None

        while current:

            p = current.particle
            p.x += p.vx
            p.y += p.vy
            p.life -= 1

            if p.life <= 0:
                if prev:
                    prev.next = current.next
                else:
                    self.head = current.next

                if current == self.tail:
                    self.tail = prev

                self.size -= 1
            else:
                prev = current

            current = current.next
# ---------------------------
# Particle System Class (Linked List-based)
# ---------------------------        
class ParticleSystemLinkedList:
    def __init__(self):
        self.particles = ParticleLinkedList()

    def spawn(self, n):
        for _ in range(n):
            p = Particle(
                x=random.random(),
                y=random.random(),
                vx=random.uniform(-1, 1),
                vy=random.uniform(-1, 1),
                life=random.randint(50, 100)
            )
            self.particles.append(p)

    def update(self):
        self.particles.update()

    def count(self):
        return self.particles.size        
        
# ---------------------------
# Particle System Class (Native Python Array-based)
# ---------------------------
class ParticleSystem:
    def __init__(self):
        self.particles = []

    # CREATE
    def spawn(self, n):
        for _ in range(n):
            p = Particle(
                x=random.random(),
                y=random.random(),
                vx=random.uniform(-1, 1),
                vy=random.uniform(-1, 1),
                life=random.randint(50, 100)
            )
            self.particles.append(p)

    # READ + UPDATE + DELETE
    def update(self):
        alive_particles = []

        for p in self.particles:
            p.update()
            if p.life > 0:
                alive_particles.append(p)

        self.particles = alive_particles

# ---------------------------
# Formal Benchmark System V.2
# ---------------------------

def benchmark_system(system_class, name, N, steps=60):
    system = system_class()   
    system.spawn(N)
    
    start = time.perf_counter()

    for _ in range(steps):
        system.update()

    end = time.perf_counter()
    
    elapsed_time = end - start # Particle Updates Per Second (PPS)
    pps = (N * steps) / elapsed_time if elapsed_time > 0 else 0

    return {
        "name": name,
        "N": N,
        "time": end - start,
        "remaining": system.count() if hasattr(system, "count") else len(system.particles),
        "pps": pps
    }
    
def save_results_csv(results, filename="benchmark_results.csv"):
    with open(filename, "w", newline="") as f:
        writer = csv.writer(f)

        writer.writerow(["System", "N", "Time(s)", "Remaining", "PPS"])

        for r in results:
            writer.writerow([
                r["name"],
                r["N"],
                r["time"],
                r["remaining"],
                r["pps"]
            ])
            
#-----APPLY BENCHMARKS------  
  
sizes = [10000, 100000, 500000, 1000000]

results = []

for N in sizes:
    print(f"\n--- Benchmarking N = {N} ---")

    results.append(benchmark_system(ParticleSystem, "PyArray", N))
    results.append(benchmark_system(ParticleSystemLinkedList, "LinkedList", N))
    results.append(benchmark_system(ParticleSystemNumPy, "VectArray", N))
    
#----OUTPUT TABLE---------

print("\nResults:")
print(f"{'System':<12} {'N':<10} {'Time (s)':<12} {'Remaining':<10} {'PPS':<10}")
print("-" * 60)

for i, r in enumerate(results, start=1):
    print(f"{r['name']:<12} {r['N']:<10} {r['time']:<12.6f} {r['remaining']:<10} {r['pps']:<10.2f}")

    if i % 3 == 0:
        print("-" * 60)
        
#------DATA CATALOGUING-----

save_results_csv(results)

#------PLOT RESULTS--------

df = pd.read_csv("benchmark_results.csv")

plt.figure()

systems = df["System"].unique()

for system in systems:
    subset = df[df["System"] == system]

    plt.plot(
        subset["N"],
        subset["PPS"],
        marker="o",
        label=system
    )

plt.xlabel("Number of Particles (N)")
plt.ylabel("Particle Updates Per Second (PPS)")
plt.title("Particle System Performance Scaling")
plt.legend()
plt.grid(True)

plt.show()