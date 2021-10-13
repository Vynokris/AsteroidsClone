#include "game.h"


int get_power_of_two(int x)
{
    return (int)pow(2, (int)(log(x)/log(2)));
}



void game_init(Game* game)
{
    // Set the game over trigger to false.
    game->game_over_trigger = false;

    // Set the starting time of the game.
    game->start_time = time(NULL);

    // Set the ending time of the game.
    game->end_time = 0;

    // Initialize the score.
    game->score = 0;

    // Initialize the score multiplier.
    game->multiplier = 1;

    // Initialize the player.
    player_init(&game->player);

    // Initialize the bullets.
    bullet_init(game->bullets);

    // Initilaize the asteroids.
    asteroid_init(game->asteroids);

    // Initialize the asteroid spawn speed.
    game->asteroid_spawn_speed = 300;

    // Initialize the asteroid spawn delay.
    game->asteroid_spawn_delay = GetRandomValue(game->asteroid_spawn_speed, game->asteroid_spawn_speed + 100);

    // Spawn a few asteroids.
    for (int i = 0; i < 5; i++) {
        spawn_asteroid(game->asteroids);
    }

    // Initialize the game's ui.
    ui_init(&game->ui);
}


void game_update(Game* game)
{
    // Update the asteroids.
    asteroid_update(game->asteroids);

    if (!is_game_over(game)) 
    {
        // Spawn asteroids.
        if (game->asteroid_spawn_delay <= 0) {
            spawn_asteroid(game->asteroids);
            game->asteroid_spawn_delay = GetRandomValue(200, 400);
        }
        game->asteroid_spawn_delay--;

        // Update the player.
        player_update(&game->player, game->bullets);

        // Update the bullets.
        bullet_update(game->bullets, &game->multiplier);

        // Check for collisions and act accordingly.
        player_collision(&game->player, game->asteroids, &game->multiplier);
        bullet_collision(game->bullets, game->asteroids, &game->score, &game->multiplier);
    }

    // Stop the game timer upon game over.
    else if (!game->game_over_trigger) {
        game->end_time = time(NULL);
        game->game_over_trigger = true;
    }

    // While the game is over, check for keyboard input to restart.
    else if (GetKeyPressed()) {
        game_save_score(game);
        game_init(game);
    }
}


void game_render(Game* game)
{
    // Draw the ui.
    ui_draw(game);

    // Draw the asteroids.
    asteroid_draw(game->asteroids);

    if (!is_game_over(game))
    {
        // Draw the bullets.
        bullet_draw(game->bullets);

        // Draw the player.
        player_draw(&game->player);
    }
}


bool is_game_over(Game* game)
{
    return game->player.hp <= 0;
}


void game_save_score(Game* game)
{
    // Get the player's highscores in strings.
    FILE* f = fopen("player_data/highscores.bin", "rb");
    int highscores[5][2];
    fread(highscores, sizeof(int) * 2, 5, f);
    fclose(f);

    // Find the place to insert the player's highscore.
    int highscore_insert_i = 0;
    for (int i = 4; i >= 0; i--) {
        if (game->score > highscores[i][0] && highscores[i][0] != 0) {
            highscore_insert_i = i;
        }
    }

    // Insert the player's score.
    int last_score[2] = { 0, 0 };
    bool move = false;
    for (int i = 0; i < 5; i++) 
    {
        last_score[0] = highscores[i][0]; last_score[1] = highscores[i][1];

        if (i == highscore_insert_i) {
            highscores[i][0] = game->score;
            highscores[i][1] = game->end_time - game->start_time;
            move = true;
            i++;
        }
        
        if (move) {
            highscores[i][0] = last_score[0]; highscores[i][1] = last_score[1];
        }
    }
    
    // Save the player's score.
    f = fopen("player_data/highscores.bin", "wb");
    fwrite(highscores, sizeof(int) * 2, 5, f);
    fclose(f);
}