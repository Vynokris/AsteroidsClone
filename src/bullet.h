#pragma once

#include "raylib.h"
#include "raymath.h"

#define BULLET_MAX_AMOUNT 3
#define BULLET_SPEED 12
#define BULLET_LIFESPAN 240


// Structure for a bullet.
typedef struct Bullet {
    int lifespan;
    float rotation;
    Vector2 pos;
} Bullet;


// Initialises all the bullets.
void bullet_init(Bullet* bullets);

// Update all the bullets of the given array.
void bullet_update(Bullet* bullet, int* multiplier);

// Draw all the bullets of the given array.
void bullet_draw(Bullet* bullets);