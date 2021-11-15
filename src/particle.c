#include "particle.h"

#include "stdio.h"

// Initializes all the given particles.
void particle_init(Particle* particles)
{
    for (int i = 0; i < PARTICLE_MAX_AMOUNT; i++)
    {
        particles[i].max_lifespan = 0;
        particles[i].lifespan = 0;
        particles[i].sides = 0;
        particles[i].sides = PARTICLE_FILLED;
        particles[i].rotation = 0;
        particles[i].rotation_speed = 0;
        particles[i].min_size = 0;
        particles[i].max_size = 0;
        particles[i].pos = Vector2Zero();
        particles[i].velocity = Vector2Zero();
        particles[i].color = (Vector3){ 0, 0, 0 };
    }
}

// Spawns a particle at the given position, with the given initial velocity and new velocity.
void particle_spawn(Particle* particles, MyVector2 pos, MyVector2 velocity, int max_lifespan, int sides, double min_size, double max_size, ParticleStyle style, Vector3 color)
{
    for (int i = 0; i < PARTICLE_MAX_AMOUNT; i++)
    {
        if (particles[i].lifespan <= 0)
        {
            particles[i].pos = pos;
            particles[i].velocity = velocity;
            particles[i].max_lifespan = max_lifespan;
            particles[i].lifespan = max_lifespan;
            particles[i].sides = sides;
            particles[i].min_size = min_size;
            particles[i].max_size = max_size;
            particles[i].rotation = (float)(GetRandomValue(0, 360) * PI / 180);
            particles[i].rotation_speed = (float)(GetRandomValue(1, 30)) * PI / 180;
            particles[i].style = style;
            particles[i].color = color;
            break;
        }
    }
}

// Updates all the given particles.
void particle_update(Particle* particles)
{
    for (int i = 0; i < PARTICLE_MAX_AMOUNT; i++) {
        if (particles[i].lifespan > 0) 
        {
            particles[i].pos.x += particles[i].velocity.x;
            particles[i].pos.y += particles[i].velocity.y;
            particles[i].rotation += particles[i].rotation_speed;
            particles[i].lifespan--;
        }
    }
}

// Draws all the given particles on screen.
void particle_draw(Particle* particles)
{
    for (int i = 0; i < PARTICLE_MAX_AMOUNT; i++) {
        if (particles[i].lifespan > 0) 
        {
            // Draw a polygon filled with the particle color.
            if (particles[i].style == PARTICLE_FILLED) {
                DrawPoly(toRayVec(particles[i].pos), 
                        particles[i].sides, 
                        remap(particles[i].lifespan, particles[i].max_lifespan, 0, particles[i].min_size, particles[i].max_size), 
                        particles[i].rotation, 
                        (Color){ particles[i].color.x, 
                                 particles[i].color.y, 
                                 particles[i].color.z, 
                                 remap(particles[i].lifespan, particles[i].max_lifespan, 0, 255, 0) });
            }

            // Draw a polygon's lines in the particle's color.
            else if (particles[i].style == PARTICLE_LINES) {
                DrawPolyLines(toRayVec(particles[i].pos), 
                              particles[i].sides, 
                              remap(particles[i].lifespan, particles[i].max_lifespan, 0, particles[i].min_size, particles[i].max_size), 
                              particles[i].rotation, 
                              (Color){ particles[i].color.x, 
                                       particles[i].color.y, 
                                       particles[i].color.z, 
                                       remap(particles[i].lifespan, particles[i].max_lifespan, 0, 255, 0) });
            }

            // Draw a polygon's lines in the particle's color, filled with black.
            else if (particles[i].style == PARTICLE_LINES_FILLED) {
                DrawPoly(toRayVec(particles[i].pos), 
                        particles[i].sides, 
                        remap(particles[i].lifespan, particles[i].max_lifespan, 0, particles[i].min_size, particles[i].max_size), 
                        particles[i].rotation, 
                        (Color){ 0, 0, 0, remap(particles[i].lifespan, particles[i].max_lifespan, 0, 255, 0) });
                DrawPolyLines(toRayVec(particles[i].pos), 
                              particles[i].sides, 
                              remap(particles[i].lifespan, particles[i].max_lifespan, 0, particles[i].min_size, particles[i].max_size), 
                              particles[i].rotation, 
                              (Color){ particles[i].color.x, 
                                       particles[i].color.y, 
                                       particles[i].color.z, 
                                       remap(particles[i].lifespan, particles[i].max_lifespan, 0, 255, 0) });
            }
        }
    }
}