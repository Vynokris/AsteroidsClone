#pragma once

#include <stdarg.h>
#include "raylib.h"
#include "my_math.h"

#include "define.h"

// Enum for the particle styles.
typedef enum ParticleStyle {
    PARTICLE_FILLED,
    PARTICLE_LINES,
    PARTICLE_LINES_FILLED,
} ParticleStyle;

// Structure for particles.
typedef struct Particle {
    int max_lifespan;
    int lifespan;
    int sides;
    ParticleStyle style;
    double rotation;
    double rotation_speed;
    double min_size;
    double max_size;
    MyVector2 pos;
    MyVector2 velocity;
    Color color;
} Particle;


// Initializes all the given particles.
void particle_init(Particle* particles);

// Spawns a particle at the given position, with the given initial velocity and new velocity.
void particle_spawn(Particle* particles, MyVector2 pos, MyVector2 velocity, int max_lifespan, int sides, double min_size, double max_size, ParticleStyle style, Color color);

// Updates all the given particles.
void particle_update(Particle* particles);

// Draws all the given particles on screen.
void particle_draw(Particle* particles, double scale);

// Renders all the particles of all the given particle arrays.
void particle_draw_all(int frames_till_beat, int array_amount, ...);