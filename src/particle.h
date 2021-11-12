#pragma once

#include "raylib.h"
#include "my_math.h"

#define PARTICLE_MAX_AMOUNT 256
#define PARTICLE_LIFESPAN 120

// Structure for particles.
typedef struct Particle {
    int lifespan;
    double rotation;
    double rotation_speed;
    MyVector2 pos;
    MyVector2 velocity;
} Particle;


// Initializes all the given particles.
void particle_init(Particle* particles);

// Spawns a particle at the given position, with the given initial velocity and new velocity.
void particle_spawn(Particle* particles, MyVector2 pos, MyVector2 initial_velocity, MyVector2 new_velocity);

// Updates all the given particles.
void particle_update(Particle* particles);

// Draws all the given particles on screen.
void particle_draw(Particle* particles);