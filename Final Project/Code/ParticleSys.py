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
# Particle Node Class (Linked List)
# ---------------------------
class Node:
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
            p.update()   # Particle method

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
# Simple Benchmark Run (Array vs Linked List)
# ---------------------------
print("Spawning particles...")

ps_array = ParticleSystem()
ps_ll = ParticleSystemLinkedList()

ps_array.spawn(100000)
ps_ll.spawn(100000)

# ---------------- ARRAY ----------------
print("\nArray system updating...")
start = time.perf_counter()

for _ in range(60):
    ps_array.update()

end = time.perf_counter()

print(f"Array time: {end - start:.6f} seconds")
print(f"Array remaining: {len(ps_array.particles)}")

# ---------------- LINKED LIST ----------------
print("\nLinked list system updating...")
start = time.perf_counter()

for _ in range(60):
    ps_ll.update()

end = time.perf_counter()

print(f"Linked list time: {end - start:.6f} seconds")
print(f"Linked list remaining: {ps_ll.count()}")