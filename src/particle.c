#include "particle.h"

// Initializes all the given particles.
void particle_init(Particle* particles)
{
    for (int i = 0; i < PARTICLE_MAX_AMOUNT; i++)
    {
        particles[i].lifespan = 0;
        particles[i].rotation = 0;
        particles[i].rotation_speed = 0;
        particles[i].pos = Vector2Zero();
        particles[i].velocity = Vector2Zero();
    }
}

// Spawns a particle at the given position, with the given initial velocity and new velocity.
void particle_spawn(Particle* particles, MyVector2 pos, MyVector2 initial_velocity, MyVector2 new_velocity)
{
    for (int i = 0; i < PARTICLE_MAX_AMOUNT; i++)
    {
        if (particles[i].lifespan <= 0)
        {
            particles[i].lifespan = PARTICLE_LIFESPAN;
            particles[i].rotation = (float)(GetRandomValue(0, 360) * PI / 180);
            particles[i].rotation_speed = (float)(GetRandomValue(1, 30)) * PI / 180;
            particles[i].pos = pos;
            particles[i].velocity = Vector2Add(initial_velocity, new_velocity);
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
            DrawPoly(toRayVec(particles[i].pos), 5, 3, particles[i].rotation, (Color){ 255, 255, 255, remap(particles[i].lifespan, 0, PARTICLE_LIFESPAN, 0, 255) });
        }
    }
}