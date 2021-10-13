#pragma once

#include <raylib.h>
#include <raymath.h>


typedef struct Ui {
    // Ui text sizes.
    float score_size;
    float timer_size;
    float life_size;
    float bullet_size;

    // Game over text sizes.
    float game_over_size;
    float info_size;
    float highscore_size;
    float restart_size;
} Ui;


#include "game.h"
typedef struct Game Game;


// Initialize the ui structure.
void ui_init(Ui* ui);

// Render the game ui: score, multiplier, lives and bullets.
void ui_draw(Game* game);

// Show the game over screen.
void game_over_screen(Game* game);