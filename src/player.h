#pragma once

#include "raylib.h"
#include "my_math.h"
#include <stdbool.h>
#include <stdio.h>

#include "define.h"
#include "particle.h"
#include "bullet.h"


// Structure for the player.
typedef struct Player {
    bool has_shot_this_beat;
    int hp;
    int invulnerable; // Refers to the number of frames of invulnerability left.
    float rotation;
    double scale;
    MyVector2 pos;
    MyVector2 velocity;
    ShapeInfo shape;
    Particle particles[PARTICLE_MAX_AMOUNT];
} Player;


// Initializes the player's values.
void player_init(Player* player);

// Makes the player shoot a bullet.
void player_shoot(Player* player, Bullet* bullets, bool powerful);

// Updates the player's shape.
void player_shape_update(Player* player);

// Updates the player's position.
void player_update(Player* player, Bullet* bullets, int frames_till_beat);

// Makes the player invulnerable for a few seconds.
void player_invulnerability(Player* player);

// Respawns the player in the center of the screen and makes him invulnerable for a few seconds.
void player_respawn(Player* player);

// Draws the player on screen.
void player_draw(Player* player);