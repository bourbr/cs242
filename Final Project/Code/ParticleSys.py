import random
import time

# ---------------------------
# Particle Class Definition
# ---------------------------
class Particle:
    def __init__(self, x, y, vx, vy, life):
        self.x = x
        self.y = y
        self.vx = vx
        self.vy = vy
        self.life = life

    def update(self):
        self.x += self.vx
        self.y += self.vy
        self.life -= 1


# ---------------------------
# Particle System Class (Array-based)
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
# Simple Benchmark Run
# ---------------------------
ps = ParticleSystem()

print("Spawning particles...")
ps.spawn(100000)   # start with 100K (for now)

print("Updating particles...")
start = time.perf_counter()

for _ in range(60): # simulating 60 updates or frames
    ps.update()

end = time.perf_counter()

print(f"Update time: {end - start:.6f} seconds")
print(f"Remaining particles: {len(ps.particles)}")
