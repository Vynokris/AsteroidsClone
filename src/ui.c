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

    // Rhythm scale.
    ui->beat_scale = 1;
    ui->offbeat_scale = 1;

    // Load the ui's rendertexture.
    ui->rendertexture = LoadRenderTexture(GetMonitorWidth(0), GetMonitorHeight(0));
    SetTextureFilter(ui->rendertexture.texture, TEXTURE_FILTER_BILINEAR);
}


void ui_update(Game* game)
{
    // ----- SCALE THE UI TO BEAT TO A BPM ----- //

    // Get the ui.
    Ui* ui = &game->ui;

    // Frames counter.
    static int frame_offbeat_counter = FRAMES_PER_BEAT;
    static int frame_counter = 0;

    // Get the beat scale.
    ui->offbeat_scale = get_beat_scale(&frame_offbeat_counter, 1.05, 2);
    ui->beat_scale    = get_beat_scale(&frame_counter,         1.05, 1);
}


void ui_render(Game* game)
{
    // Render the ui on the rendertexture.
    BeginTextureMode(game->ui.rendertexture);
    {
        // Fill the rendertexture with black.
        ClearBackground((Color){ 0, 0, 0, 0 });

        // Render the UI while the game is running.
        if (!is_game_over(game)) 
        {
            // Get the game's ui.
            Ui* ui = &game->ui;

            // Set the text offset.
            float text_offset_y = - ui->score_size - ui->timer_size;

            // Draw the score multiplier.
            DrawText(TextFormat("x%d", get_power_of_two(game->multiplier)), 
                    GetMonitorWidth(0) / 2 - MeasureText(TextFormat("x%d", game->multiplier), ui->timer_size) / 2, 
                    GetMonitorHeight(0) / 2 + text_offset_y, 
                    ui->timer_size, 
                    (Color){ (game->multiplier >= 16 ? RED.r : remap(get_power_of_two(game->multiplier), 1, 8, GREEN.r, YELLOW.r)), 
                             (game->multiplier >= 16 ? RED.g : remap(get_power_of_two(game->multiplier), 1, 8, GREEN.g, YELLOW.g)),
                             (game->multiplier >= 16 ? RED.b : remap(get_power_of_two(game->multiplier), 1, 8, GREEN.b, YELLOW.b)),
                             200 });

            text_offset_y += ui->timer_size;

            // Draw the score.
            DrawText(TextFormat("%d", game->score), 
                    GetMonitorWidth(0) / 2 - MeasureText(TextFormat("%d", game->score), ui->score_size) / 2, 
                    GetMonitorHeight(0) / 2 + text_offset_y, 
                    ui->score_size, 
                    GRAY);
            
            text_offset_y += ui->score_size;
            
            // Draw the timer.
            DrawText(TextFormat("%ds", time(NULL) - game->start_time),
                    GetMonitorWidth(0) / 2 - MeasureText(TextFormat("%ds", time(NULL) - game->start_time), ui->timer_size) / 2, 
                    GetMonitorHeight(0) / 2 + text_offset_y, 
                    ui->timer_size, 
                    GRAY);
            
            text_offset_y += ui->timer_size * 2.5;

            // Draw the lives.
            Vector2 triangle_offset = { (GetMonitorWidth(0) - ((game->player.hp - 1) * ui->life_size) - ((game->player.hp - 1) * 8)) / 2,
                                         GetMonitorHeight(0) / 2 + text_offset_y };
            
            for (int i = 0; i < game->player.hp; i++) {
                if (i != game->player.hp - 1 || (!game->player.invulnerable || game->player.scale > 1.06)) {
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
        }

        // If the game is over, render the game over screen instead of the UI.
        else {
            game_over_screen(game);
        }
    }
    EndTextureMode();
}


void game_over_screen(Game* game)
{
    // Get the game's UI.
    Ui* ui = &game->ui;

    // Draw the game over text.
    DrawText("GAME OVER", 
             GetMonitorWidth(0) / 2 - MeasureText("GAME OVER", ui->game_over_size) / 2, 
             GetMonitorHeight(0) / 2 - (ui->game_over_size + 5) * 2, 
             ui->game_over_size, 
             RED);

    // Draw the final score.
    DrawText(TextFormat("Score: %d", game->score), 
             GetMonitorWidth(0) / 2 - MeasureText(TextFormat("Score: %d", game->score), ui->info_size) / 2, 
             GetMonitorHeight(0) / 2 - ui->game_over_size + 5, 
             ui->info_size, 
             WHITE);

    // Draw the game duration.
    DrawText(TextFormat("Duration: %ds", game->end_time - game->start_time), 
             GetMonitorWidth(0) / 2 - MeasureText(TextFormat("Duration: %ds", game->end_time - game->start_time), ui->info_size) / 2, 
             GetMonitorHeight(0) / 2 - ui->game_over_size + ui->info_size + 10, 
             ui->info_size, 
             WHITE);

    // Show highscores title.
    DrawText("Highscores:", 
             GetMonitorWidth(0) / 2 - MeasureText("Highscores:", game->ui.highscore_size) / 2, 
             GetMonitorHeight(0) / 2 - ui->game_over_size + ui->info_size * 4, 
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
                GetMonitorWidth(0) / 2 - MeasureText(TextFormat((highscores[i][0] > 0 ? "#%d: %d %ds" : "#%d: -"), i+1, highscores[i][0], highscores[i][1]), ui->highscore_size) / 2,
                GetMonitorHeight(0) / 2 - ui->game_over_size + ui->info_size * 4 + (ui->highscore_size + 5) * (i+1),
                ui->highscore_size,
                (highscores[i][0] == game->score ? WHITE: GRAY));
    }

    // Draw the restart promt.
    DrawText("Press any key to restart or press [Esc] to quit", 
             GetMonitorWidth(0) / 2 - MeasureText("Press any key to restart or press [Esc] to quit", ui->restart_size) / 2, 
             GetMonitorHeight(0) - ui->restart_size * 1.5, 
             ui->restart_size, 
             WHITE);
}