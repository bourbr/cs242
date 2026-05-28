import pygame
import random

# ---------------------------
# Particle
# ---------------------------

class Particle:
    __slots__ = ('x','y','vx','vy','life')

    def __init__(self, x, y, vx, vy, life):
        self.x = x
        self.y = y
        self.vx = vx
        self.vy = vy
        self.life = life


# ---------------------------
# Node
# ---------------------------

class Node:
    __slots__ = ('particle','next')

    def __init__(self, particle):
        self.particle = particle
        self.next = None


# ---------------------------
# Linked List
# ---------------------------

class ParticleLinkedList:

    def __init__(self):
        self.head = None
        self.tail = None
        self.size = 0

    def append(self, particle):

        new_node = Node(particle)

        if self.head is None:
            self.head = self.tail = new_node
        else:
            self.tail.next = new_node
            self.tail = new_node

        self.size += 1


    def update(self, width, height):

        current = self.head
        prev = None

        while current:

            p = current.particle

            # Update motion
            p.x += p.vx
            p.y += p.vy
            p.life -= 1

            # Bounce
            if p.x <= 0 or p.x >= width:
                p.vx *= -1

            if p.y <= 0 or p.y >= height:
                p.vy *= -1


            # Delete dead particles
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
# Particle System
# ---------------------------

class ParticleSystemLinkedList:

    def __init__(self):
        self.particles = ParticleLinkedList()

    def spawn(self, n, width, height):

        for _ in range(n):

            p = Particle(
                random.uniform(0,width),
                random.uniform(0,height),
                random.uniform(-1,1),
                random.uniform(-1,1),
                random.randint(250,500)
            )

            self.particles.append(p)

    def update(self,width,height):
        self.particles.update(width,height)

    def count(self):
        return self.particles.size


# ---------------------------
# Pygame Setup
# ---------------------------

pygame.init()

WIDTH=1000
HEIGHT=700

screen=pygame.display.set_mode((WIDTH,HEIGHT))
pygame.display.set_caption(
    "Linked List Particle Visualization"
)

clock=pygame.time.Clock()

font=pygame.font.SysFont(None,40)

# ---------------------------
# Spawn particles
# ---------------------------

ps=ParticleSystemLinkedList()

ps.spawn(
    50000,
    WIDTH,
    HEIGHT
)

fps_surface=None
count_surface=None

frame_counter=0

# ---------------------------
# Main Loop
# ---------------------------

fps_surface = font.render("FPS: 0", True, (0,255,0))
count_surface = font.render("Particles: 0", True, (0,255,0))

running=True

while running:

    for event in pygame.event.get():
        if event.type==pygame.QUIT:
            running=False

    ps.update(WIDTH,HEIGHT)

    screen.fill((0,0,0))

    # Traverse linked list and draw

    current=ps.particles.head

    while current:

        p=current.particle

        x=int(p.x)
        y=int(p.y)

        if (
            0<=x<WIDTH
            and
            0<=y<HEIGHT
        ):

            screen.set_at(
                (x,y),
                (255,255,255)
            )

        current=current.next


    # Update text every 10 frames

    frame_counter += 1

    if frame_counter % 10 == 0:

        fps_surface = font.render(
            f"FPS: {int(clock.get_fps())}",
            True,
            (0,255,0)
    )

        count_surface = font.render(
            f"Particles: {ps.count()}",
            True,
            (0,255,0)
    )


        if fps_surface:
            screen.blit(fps_surface, (10,10))

        if count_surface:
            screen.blit(count_surface, (10,50))
            pygame.display.flip()

    clock.tick(60)

pygame.quit()