#include "ui.h"


void ui_init(Ui* ui)
{
    // Set all of the ui's values.
    ui->score_size = 200;
    ui->timer_size = 50;
    ui->life_size = 40;
    ui->bullet_size = 2;

    // Set all of the game over ui's values.
    ui->game_over_size = 120;
    ui->info_size = 70;
    ui->highscore_size = 50;
    ui->restart_size = 30;
}


void ui_draw(Game* game)
{
    // Render the UI while the game is running.
    if (!is_game_over(game)) {
        // Get the game's ui.
        Ui* ui = &game->ui;

        // Set the text offset.
        float text_offset_y = - ui->score_size - ui->timer_size;

        // Draw the score multiplier.
        DrawText(TextFormat("x%d", get_power_of_two(game->multiplier)), 
                GetScreenWidth() / 2 - MeasureText(TextFormat("x%d", game->multiplier), ui->timer_size) / 2, 
                GetScreenHeight() / 2 + text_offset_y, 
                ui->timer_size, 
                (Color){ (game->multiplier >= 16 ? RED.r : Remap(get_power_of_two(game->multiplier), 1, 8, GREEN.r, YELLOW.r)), 
                         (game->multiplier >= 16 ? RED.g : Remap(get_power_of_two(game->multiplier), 1, 8, GREEN.g, YELLOW.g)),
                         (game->multiplier >= 16 ? RED.b : Remap(get_power_of_two(game->multiplier), 1, 8, GREEN.b, YELLOW.b)),
                         200 });

        text_offset_y += ui->timer_size;

        // Draw the score.
        DrawText(TextFormat("%d", game->score), 
                GetScreenWidth() / 2 - MeasureText(TextFormat("%d", game->score), ui->score_size) / 2, 
                GetScreenHeight() / 2 + text_offset_y, 
                ui->score_size, 
                GRAY);
        
        text_offset_y += ui->score_size;
        
        // Draw the timer.
        DrawText(TextFormat("%ds", time(NULL) - game->start_time),
                GetScreenWidth() / 2 - MeasureText(TextFormat("%ds", time(NULL) - game->start_time), ui->timer_size) / 2, 
                GetScreenHeight() / 2 + text_offset_y, 
                ui->timer_size, 
                GRAY);
        
        text_offset_y += ui->timer_size * 2.5;

        // Draw the lives.
        Vector2 triangle_offset = { (GetScreenWidth() - ((game->player.hp - 1) * ui->life_size) - ((game->player.hp - 1) * 8)) / 2,
                                    GetScreenHeight() / 2 + text_offset_y };
        
        for (int i = 0; i < game->player.hp; i++) {
            if (i != game->player.hp - 1 || (game->player.invulnerable / 10) % 2 == 0) {
                Vector2 points[3] = 
                {
                    { (triangle_offset.x + (i * (ui->life_size + 8))) + sinf(-PI/2) * ui->life_size / 2, 
                    triangle_offset.y - cosf(-PI/2) * ui->life_size / 2 },

                    { (triangle_offset.x + (i * (ui->life_size + 8))) - sinf(-PI/2) * ui->life_size / 2, 
                    triangle_offset.y + cosf(-PI/2) * ui->life_size / 2 },

                    { (triangle_offset.x + (i * (ui->life_size + 8))) + cosf(-PI/2) * ui->life_size, 
                    triangle_offset.y + sinf(-PI/2) * ui->life_size },
                };
                DrawTriangleLines(points[0], points[1], points[2], GRAY);
            }
        }

        text_offset_y += ui->life_size;

        // Draw the bullets.
        int num_bullets = 0;
        for (int i = 0; i < sizeof(game->bullets) / sizeof(game->bullets[0]); i++) {
            if (game->bullets[i].lifespan == 0) {
                num_bullets++;
            }
        }

        Vector2 bullet_offset = { (GetScreenWidth() - ((num_bullets - 1) * ui->bullet_size) - ((num_bullets - 1) * 16)) / 2, 
                                GetScreenHeight() / 2 + text_offset_y };

        for (int i = 0; i < num_bullets; i++) {
            DrawCircle(bullet_offset.x + i * (ui->bullet_size + 16), 
                       bullet_offset.y, 
                       ui->bullet_size, 
                       GRAY);
        }
    }

    // If the game is over, render the game over screen instead of the UI.
    else {
        game_over_screen(game);
    }
}


void game_over_screen(Game* game)
{
    // Get the game's UI.
    Ui* ui = &game->ui;

    // Draw the game over text.
    DrawText("GAME OVER", 
             GetScreenWidth() / 2 - MeasureText("GAME OVER", ui->game_over_size) / 2, 
             GetScreenHeight() / 2 - (ui->game_over_size + 5) * 2, 
             ui->game_over_size, 
             RED);

    // Draw the final score.
    DrawText(TextFormat("Score: %d", game->score), 
             GetScreenWidth() / 2 - MeasureText(TextFormat("Score: %d", game->score), ui->info_size) / 2, 
             GetScreenHeight() / 2 - ui->game_over_size + 5, 
             ui->info_size, 
             WHITE);

    // Draw the game duration.
    DrawText(TextFormat("Duration: %ds", game->end_time - game->start_time), 
             GetScreenWidth() / 2 - MeasureText(TextFormat("Duration: %ds", game->end_time - game->start_time), ui->info_size) / 2, 
             GetScreenHeight() / 2 - ui->game_over_size + ui->info_size + 10, 
             ui->info_size, 
             WHITE);

    // Show highscores title.
    DrawText("Highscores:", 
             GetScreenWidth() / 2 - MeasureText("Highscores:", game->ui.highscore_size) / 2, 
             GetScreenHeight() / 2 - ui->game_over_size + ui->info_size * 4, 
             ui->highscore_size, 
             GRAY);

    // Get highscores.
    FILE* f = fopen("player_data/highscores.bin", "rb");
    int highscores[5][2];
    fread(highscores, sizeof(int) * 2, 5, f);
    fclose(f);

    // Show highscores.
    for (int i = 0; i < 5; i++) 
    {
        char line_i[64];
        fgets(line_i, 64, f);

        DrawText(TextFormat((highscores[i][0] > 0 ? "#%d: %d %ds" : "#%d: -"), i+1, highscores[i][0], highscores[i][1]),
                GetScreenWidth() / 2 - MeasureText(TextFormat((highscores[i][0] > 0 ? "#%d: %d %ds" : "#%d: -"), i+1, highscores[i][0], highscores[i][1]), ui->highscore_size) / 2,
                GetScreenHeight() / 2 - ui->game_over_size + ui->info_size * 4 + (ui->highscore_size + 5) * (i+1),
                ui->highscore_size,
                (highscores[i][0] == game->score ? WHITE: GRAY));
    }

    // Draw the restart promt.
    DrawText("Press any key to restart or press [Esc] to quit", 
             GetScreenWidth() / 2 - MeasureText("Press any key to restart or press [Esc] to quit", ui->restart_size) / 2, 
             GetScreenHeight() - ui->restart_size * 1.5, 
             ui->restart_size, 
             WHITE);
}