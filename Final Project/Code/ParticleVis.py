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

        self.life = np.random.randint(1000, 2000, n).astype(np.int32)

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
ps.spawn(50000, WIDTH, HEIGHT)

# ---------------------------
# Main Loop
# ---------------------------
running = True

pygame.font.init()
font = pygame.font.SysFont(None, 70)

while running:
    
    # Handle quit
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False

    # Update particles
    ps.update(WIDTH, HEIGHT)
        

    # Clear screen
    screen.fill((0, 0, 0))

    # Draw particles
    for i in range(ps.count()):
        pygame.draw.circle(
            screen,
            (255, 255, 255),
            (int(ps.x[i]), int(ps.y[i])),
            1
        )
    
    #FPS Counter    
    fps_text = font.render(
    f"FPS: {int(clock.get_fps())}",
    True,
    (0, 255, 0)
    )

    screen.blit(fps_text, (10, 10))
    
    # Particle Counter
    count_text = font.render(
    f"Particles: {ps.count()}",
    True,
    (0, 255, 0)
    )

    screen.blit(count_text, (10, 70))


    # Update display
    pygame.display.flip()

    # FPS limit
    clock.tick(60)

pygame.quit()