#pragma once

#include "raylib.h"
#include "my_math.h"
#include <stdio.h>
#include <string.h>

#include "define.h"

// Enum for the two asteroid types.
typedef enum AsteroidType {
    A_DESTROYED,
    A_SMALL,
    A_MEDIUM,
    A_LARGE,
} AsteroidType;

// Structure for the asteroid.
typedef struct Asteroid {
    int size;
    int sides;
    float rotation;
    float rotation_speed;
    double scale;
    AsteroidType type;
    MyVector2 pos;
    MyVector2 velocity;
} Asteroid;


// Initialises the all the asteroids.
void asteroid_init(Asteroid* asteroids);

// Spawns a new asteroid offscreen that moves towards the screen
void spawn_asteroid(Asteroid* asteroids);

// Updates an asteroid's shape.
void asteroid_shape_update(Asteroid* asteroid);

// Updates the position of all the asteroids in the given array.
void asteroid_update(Asteroid* asteroids, int frames_till_beat);

// Breaks the given asteroid into two smaller ones.
void asteroid_break(Asteroid* asteroids, int break_i);

// Draws all the asteroids in the given array.
void asteroid_draw(Asteroid* asteroids);