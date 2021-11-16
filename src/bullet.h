#pragma once

#include "raylib.h"
#include "my_math.h"

#include "define.h"


// Structure for a bullet.
typedef struct Bullet {
    int lifespan;
    float rotation;
    MyVector2 pos;
} Bullet;


// Initialises all the bullets.
void bullet_init(Bullet* bullets);

// Update all the bullets of the given array.
void bullet_update(Bullet* bullet, int* multiplier);

// Draw all the bullets of the given array.
void bullet_draw(Bullet* bullets);