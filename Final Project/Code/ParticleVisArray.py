import pygame
import numpy as np

# ---------------------------
# VectArray (NumPy) Particle System
# ---------------------------
class ParticleSystemNumPy:
    def __init__(self):
        self.x = np.array([], dtype=np.float32)
        self.y = np.array([], dtype=np.float32)

        self.vx = np.array([], dtype=np.float32)
        self.vy = np.array([], dtype=np.float32)

        self.life = np.array([], dtype=np.int32)

    def spawn(self, n, width, height):
        self.x = np.random.uniform(0, width, n).astype(np.float32)
        self.y = np.random.uniform(0, height, n).astype(np.float32)

        self.vx = np.random.uniform(-1, 1, n).astype(np.float32)
        self.vy = np.random.uniform(-1, 1, n).astype(np.float32)

        self.life = np.random.randint(500, 1000, n).astype(np.int32)

    def update(self, width, height):
        self.x += self.vx
        self.y += self.vy

        self.life -= 1

        alive = self.life > 0

        self.x = self.x[alive]
        self.y = self.y[alive]

        self.vx = self.vx[alive]
        self.vy = self.vy[alive]

        self.life = self.life[alive]
        
        self.vx[self.x <= 0] *= -1
        self.vx[self.x >= WIDTH] *= -1

        self.vy[self.y <= 0] *= -1
        self.vy[self.y >= HEIGHT] *= -1

    def count(self):
        return len(self.x)

# ---------------------------
# Pygame Setup
# ---------------------------
pygame.init()

font = pygame.font.SysFont(None, 30)

WIDTH = 1000
HEIGHT = 700

screen = pygame.display.set_mode((WIDTH, HEIGHT))
pygame.display.set_caption("Particle System Visualization")

clock = pygame.time.Clock()

# ---------------------------
# Particle System
# ---------------------------
ps = ParticleSystemNumPy()
ps.spawn(1000000, WIDTH, HEIGHT)

# ---------------------------
# Main Loop
# ---------------------------
running = True

pygame.font.init()
font = pygame.font.SysFont(None, 70)

while running:
    
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False

    # Update particles
    ps.update(WIDTH, HEIGHT)       

    # Clear screen
    screen.fill((0,0,0))

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
       
    fps_text = font.render(
    f"FPS: {int(clock.get_fps())}",
    True,
    (0, 0, 255)
    )

    screen.blit(fps_text, (10, 10))
    
    count_text = font.render(
    f"Particles: {ps.count()}",
    True,
    (0, 0, 255)
    )

    screen.blit(count_text, (10, 70))

    pygame.display.flip()

    clock.tick(60)

pygame.quit()