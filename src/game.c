#include "game.h"


int get_power_of_two(int x)
{
    return (int)pow(2, (int)(log(x)/log(2)));
}


void screen_shake(Game* game, bool start_shake, int duration_multiplier, int new_intensity)
{
    static int remaining_frames = 0;
    static int intensity = 1;

    if (start_shake)
    {
        remaining_frames = SCREEN_SHAKE_DURATION;
        intensity = new_intensity;
    }
    
    if (remaining_frames > 0)
    {
        remaining_frames--;
        game->screen_offset[0] = GetRandomValue(-5 * intensity, 5 * intensity);
        game->screen_offset[1] = GetRandomValue(-5 * intensity, 5 * intensity);
    }
    else if (game->screen_offset[0] != 0 || game->screen_offset[1] != 0)
    {
        game->screen_offset[0] = 0;
        game->screen_offset[1] = 0;
    }
}



void game_init(Game* game)
{
    // Set the game over trigger to false.
    game->game_over_trigger = false;

    // Don't show the debug keys.
    game->show_debug_keys = false;

    // Initialize the current beat value.
    game->current_beat = 0;

    // Initialize the number of frames intil the next beat.
    game->frames_till_beat = 0;

    // Set the starting time of the game.
    game->start_time = time(NULL);

    // Set the ending time of the game.
    game->end_time = 0;

    // Initialize the score.
    game->score = 0;

    // Initialize the score multiplier.
    game->multiplier = 1;

    // Initialize the screen offset values.
    game->screen_offset[0] = 0; game->screen_offset[1] = 0;

    // Initialize the player.
    player_init(&game->player);

    // Initialize the bullets.
    bullet_init(game->bullets);

    // Initilaize the asteroids.
    asteroid_init(game->asteroids);

    // Initialize the asteroid particles.
    particle_init(game->asteroid_particles);

    // Initialize the asteroid spawn speed.
    game->asteroid_spawn_speed = 300;

    // Initialize the asteroid spawn delay.
    game->asteroid_spawn_delay = GetRandomValue(game->asteroid_spawn_speed, game->asteroid_spawn_speed + 100);

    // Spawn a few asteroids.
    for (int i = 0; i < 5; i++) {
        spawn_asteroid(game->asteroids);
    }

    // Load the game's rendertexture.
    game->rendertexture = LoadRenderTexture(GetMonitorWidth(0), GetMonitorHeight(0));

    // Initialize the game's ui.
    ui_init(&game->ui);
}


void game_update(Game* game)
{
    // Update the beat variables.
    game->frames_till_beat--;
    if (game->frames_till_beat < 0) {
        game->frames_till_beat = FRAMES_PER_BEAT;
        game->current_beat++;
    }

    // Update the asteroids.
    asteroid_update(game->asteroids, game->frames_till_beat);

    // Update the particles' particles.
    particle_update(game->asteroid_particles);

    // Update the player's particles.
    particle_update(game->player.particles);

    // Update the ui.
    ui_update(game);

    // Update the screen shake.
    screen_shake(game, false, 1, 1);

    if (!is_game_over(game)) 
    {
        // Spawn asteroids.
        if (game->asteroid_spawn_delay <= 0) {
            spawn_asteroid(game->asteroids);
            game->asteroid_spawn_delay = GetRandomValue(200, 400);
        }
        game->asteroid_spawn_delay--;

        // Update the player.
        player_update(&game->player, game->bullets, game->frames_till_beat);

        // Update the bullets.
        bullet_update(game->bullets, &game->multiplier);

        // Check for collisions and act accordingly.
        if (player_collision(&game->player, game->asteroids, &game->multiplier)) {
            screen_shake(game, true, 2, 2);
        }
        if (bullet_collision(game->bullets, game->asteroids, &game->score, &game->multiplier, game->asteroid_particles)) {
            screen_shake(game, true, 1, 1);
        }
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
        if (IsKeyPressed(KEY_KP_ENTER))
            game->show_debug_keys = !(game->show_debug_keys);

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
        if (IsKeyPressed(KEY_KP_ADD))
            spawn_asteroid(game->asteroids);

        // Give hp key.
        if (IsKeyPressed(KEY_KP_DECIMAL))
            game->player.hp++;

        // Give score key.
        if (IsKeyDown(KEY_KP_1))
            game->score += 10 * game->multiplier;

        // Increment multiplier key.
        if (IsKeyPressed(KEY_KP_2))
            game->multiplier++;

        // Increment time key.
        if (IsKeyPressed(KEY_KP_3))
            game->start_time-=10;

        // Invulnerability key.
        if (IsKeyPressed(KEY_KP_MULTIPLY))
            game->player.invulnerable += FRAMES_PER_BEAT * 6;
    }
}


void game_render(Game* game)
{
    BeginTextureMode(game->rendertexture);
    {
        ClearBackground((Color){ 0, 0, 0, 0 });

        // Draw the asteroids' particles.
        particle_draw_all(game->frames_till_beat, 2, game->asteroid_particles, game->player.particles);

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
                DrawText("[KP_ENTER]: Show this cheat menu\n[KP_0]: Kill player\n[KP_DECIMAL]: Give hp\n[KP_MULTIPLY]: Add 5s of invulnerability\n[KP_1]: Give score\n[KP_2]: Increment multiplier\n[KP_3]: Add game time\n[KP_ADD]: Spawn asteroid\n[KP_SUBSTRACT]: Clear highscores",
                        10, 30, 20, GRAY);
            }
        }
    }
    EndTextureMode();
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
    int insert_i = -1;
    bool insert = false;
    bool replace = false;
    for (int i = 4; i >= 0; i--) 
    {
        if (i == 0 || highscores[i-1][0] != 0) {
            // If the player got a score that is already on the highscore board.
            if (game->score == highscores[i][0]) {
                if (game->end_time - game->start_time < highscores[i][1])
                    replace = true;
                else
                    insert = false;
                break;
            }
            // If the player got a highscore that isn't on the score board.
            else if (game->score > highscores[i][0]) {
                insert_i = i;
                insert = true;
            }
            else
                break;
        }
    }

    // Insert the player's score into the array.
    if (insert) 
    {
        // Shift the scores to the right in the array.
        if (!replace) {
            for (int i = 4; i >= insert_i && i > 0; i--) {
                highscores[i][0] = highscores[i-1][0];
                highscores[i][1] = highscores[i-1][1];
            }
        }

        // Insert the player's score.
        highscores[insert_i][0] = game->score;
        highscores[insert_i][1] = game->end_time - game->start_time;
    }
    
    // Save the player's score.
    f = fopen("player_data/highscores.bin", "wb");
    fwrite(highscores, sizeof(int) * 2, 5, f);
    fclose(f);
}