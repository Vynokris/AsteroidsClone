#pragma once

#include "raylib.h"
#include <stdbool.h>
#include <time.h>

#include "player.h"
#include "asteroid.h"
#include "bullet.h"
#include "collisions.h"
#include "ui.h"


typedef struct Game {
    bool game_over_trigger;
    bool show_debug_keys;
    int score;
    int multiplier;
    int asteroid_spawn_speed;
    int asteroid_spawn_delay;
    time_t start_time;
    time_t end_time;
    Player player;
    Bullet bullets[BULLET_MAX_AMOUNT];
    Asteroid asteroids[ASTEROID_MAX_AMOUNT];
    Ui ui;
} Game;


// Returns the highest power of 2 smaller than x.
int get_power_of_two(int x);


// Initializes all game values and objects.
void game_init(Game* game);

// Update all the game objects.
void game_update(Game* game);

// Listen to the debug keys.
void debug_keys(Game* game);

// Render all the game sprites.
void game_render(Game* game);

// Check if the game is over.
bool is_game_over(Game* game);

// Save the player's score.
void game_save_score(Game* game);