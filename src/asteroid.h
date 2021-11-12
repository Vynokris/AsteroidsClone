#pragma once

#include "raylib.h"
#include "my_math.h"
#include <stdio.h>
#include <string.h>


#define ASTEROID_MAX_AMOUNT 64
#define ASTEROID_SIZE(type) (type==0 ? 0 : (type==1 ? 30 : (type==2 ? 60 : 90))) // A_DESTROYED: 0, A_SMALL: 30, A_MEDIUM: 60, A_LARGE: 90
#define ASTEROID_SIDES(type) (type==0 ? 0 : (type==1 ? 5 : (type==2 ? 8 : 11))) // A_DESTROYED: 0, A_SMALL: 5, A_MEDIUM: 8, A_LARGE: 11
#define ASTEROID_SPEED(type) (type==0 ? 0.0f : (type==1 ? 2.5f : (type==2 ? 2.0f : 1.5f))) // A_DESTROYED: 0.0, A_SMALL: 2.5, A_MEDIUM: 2.0, A_LARGE: 1.5

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
    MyVector2 pos;
    MyVector2 velocity;
    AsteroidType type;
} Asteroid;


// Initialises the all the asteroids.
void asteroid_init(Asteroid* asteroids);

// Spawns a new asteroid offscreen that moves towards the screen
void spawn_asteroid(Asteroid* asteroids);

// Updates the position of all the asteroids in the given array.
void asteroid_update(Asteroid* asteroids);

// Breaks the given asteroid into two smaller ones.
void asteroid_break(Asteroid* asteroids, int break_i);

// Draws all the asteroids in the given array.
void asteroid_draw(Asteroid* asteroids);