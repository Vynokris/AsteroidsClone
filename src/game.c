#include "game.h"


int get_power_of_two(int x)
{
    return (int)pow(2, (int)(log(x)/log(2)));
}



void game_init(Game* game)
{
    // Set the game over trigger to false.
    game->game_over_trigger = false;

    // Don't show the debug keys.
    game->show_debug_keys = false;

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
        game_save_score(game);
        game->game_over_trigger = true;
    }

    // While the game is over, check for keyboard input to restart.
    else if (GetKeyPressed()) {
        game_init(game);
    }
}


void debug_keys(Game* game)
{
    if (!is_game_over(game)) {
        // Show debug buttons trigger.
        if (IsKeyPressed(KEY_KP_ENTER)) {
            game->show_debug_keys = !(game->show_debug_keys);
        }

        // Kill key.
        if (IsKeyPressed(KEY_KP_0))
            game->player.hp = 0;

        // Clear leaderboard key.
        if (IsKeyPressed(KEY_KP_SUBTRACT)) {
            int highscores[5][2] = { {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0} };
            FILE* f = fopen("player_data/highscores.bin", "wb");
            fwrite(highscores, sizeof(int) * 2, 5, f);
            fclose(f);
        }

        // Spawn asteroid key.
        if (IsKeyPressed(KEY_KP_ADD)) {
            spawn_asteroid(game->asteroids);
        }

        // Give hp key.
        if (IsKeyPressed(KEY_KP_DECIMAL)) {
            game->player.hp++;
        }

        // Give score key.
        if (IsKeyDown(KEY_KP_1)) {
            game->score += 10 * game->multiplier;
        }

        // Increment multiplier key.
        if (IsKeyPressed(KEY_KP_2)) {
            game->multiplier++;
        }

        // Increment time key.
        if (IsKeyPressed(KEY_KP_3)) {
            game->start_time-=10;
        }
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

        // Show the debug keys.
        if (game->show_debug_keys) {
            DrawText("[KP_ENTER]: Show this cheat menu\n[KP_0]: Kill player\n[KP_DECIMAL]: Give hp\n[KP_1]: Give score\n[KP_2]: Increment multiplier\n[KP_3]: Add game time\n[KP_ADD]: Spawn asteroid\n[KP_SUBSTRACT]: Clear highscores",
                     10, 30, 20, GRAY);
        }
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
    int highscore_insert_i = -1;
    int highscore_replace_i = -1;
    for (int i = 4; i >= 0; i--) {
        if (i == 0 || highscores[i-1][0] != 0) {
            if (game->score == highscores[i][0] && game->end_time - game->start_time < highscores[i][1])
                highscore_replace_i = i;
            if (highscore_replace_i == -1 && game->score > highscores[i][0])
                highscore_insert_i = i;
        }
    }

    // TODO: Figure out why there is some wierdness with score saving.

    // Insert the player's score.
    if (highscore_insert_i != -1) 
    {
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

            if (i == highscore_replace_i) {
                highscores[i][0] = game->score;
                highscores[i][1] = game->end_time - game->start_time;
                break;
            }
            
            if (move) {
                highscores[i][0] = last_score[0]; highscores[i][1] = last_score[1];
            }
        }
    }
    
    // Save the player's score.
    f = fopen("player_data/highscores.bin", "wb");
    fwrite(highscores, sizeof(int) * 2, 5, f);
    fclose(f);
}