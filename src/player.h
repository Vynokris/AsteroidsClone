#pragma once

#include "raylib.h"
#include "raymath.h"
#include <stdbool.h>

#include "bullet.h"


#define PLAYER_MAX_VELOCITY 8
#define PLAYER_SPEED 0.5


// Structure for the player.
typedef struct Player {
    int hp;
    int invulnerable; // Refers to the number of frames of invulnerability left.
    float rotation;
    Vector2 pos;
    Vector2 velocity;
} Player;


// Initializes the player's values.
void player_init(Player* player);

// Makes the player shoot a bullet.
void player_shoot(Player* player, Bullet* bullets);

// Updates the player's position.
void player_update(Player* player, Bullet* bullets);

// Makes the player invulnerable for a few seconds.
void player_invulnerability(Player* player);

// Respawns the player in the center of the screen and makes him invulnerable for a few seconds.
void player_respawn(Player* player);

// Draws the player on screen.
void player_draw(Player* player);