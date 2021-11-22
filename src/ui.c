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

    // Get the beat scale.
    ui->beat_scale    = get_beat_scale(game->frames_till_beat + FRAMES_PER_BEAT * ((game->current_beat % 2 + 1) % 2),                                                  1.05,         2);
    ui->offbeat_scale = get_beat_scale(game->frames_till_beat + FRAMES_PER_BEAT * (game->current_beat  % 2),          (game->start_time == 0 || game->player.hp <= 0 ? 1.05 : 1.15), 2);
}


void ui_render(Game* game)
{
    // Render the ui on the rendertexture.
    BeginTextureMode(game->ui.rendertexture);
    {
        // Fill the rendertexture with a transparent color.
        ClearBackground((Color){ 0, 0, 0, 0 });

        // Render the main menu.
        if (game->start_time == 0)
            main_menu_screen(game);

        // Render the UI while the game is running.
        else if (!is_game_over(game)) 
            in_game_ui(game);

        // If the game is over, render the game over screen instead of the UI.
        else
            game_over_screen(game);
    }
    EndTextureMode();
}


void main_menu_screen(Game* game)
{
    // Get the game's UI.
    Ui* ui = &game->ui;

    // Draw the title.
    DrawText("----------",  GetScreenWidth() / 2 - MeasureText("----------",  120) / 2, 340, 120, (ui->beat_scale >= 1.04 || ui->offbeat_scale >= 1.04 ? BEAT_COLOR : WHITE));
    DrawText("-----------", GetScreenWidth() / 2 - MeasureText("-----------", 120) / 2, 340, 120, (ui->beat_scale >= 1.04 || ui->offbeat_scale >= 1.04 ? BEAT_COLOR : WHITE));
    DrawText("DUBSPACE",    GetScreenWidth() / 2 - MeasureText("DUBSPACE",    120) / 2, 400, 120, (ui->beat_scale >= 1.04 || ui->offbeat_scale >= 1.04 ? BEAT_COLOR : WHITE));
    DrawText("-----------", GetScreenWidth() / 2 - MeasureText("-----------", 120) / 2, 460, 120, (ui->beat_scale >= 1.04 || ui->offbeat_scale >= 1.04 ? BEAT_COLOR : WHITE));
    DrawText("----------",  GetScreenWidth() / 2 - MeasureText("----------",  120) / 2, 460, 120, (ui->beat_scale >= 1.04 || ui->offbeat_scale >= 1.04 ? BEAT_COLOR : WHITE));

    // ----- Draw the keybindings ----- //

    // Start.
    DrawText("Start Game [space]", GetScreenWidth() / 2 - MeasureText("Start game [space]", 40) / 2, GetScreenHeight() / 2 - 20 - 30, 40, (ui->beat_scale >= 1.04 || ui->offbeat_scale >= 1.04 ? BEAT_COLOR : WHITE));

    // Quit.
    DrawText("Quit Game [escape]", GetScreenWidth() / 2 - MeasureText("Quit Game [escape]", 40) / 2, GetScreenHeight() / 2 - 20 + 30, 40, (ui->beat_scale >= 1.04 || ui->offbeat_scale >= 1.04 ? BEAT_COLOR : WHITE));

    // Fire.
    DrawText("Fire [space]", GetScreenWidth() / 2 - MeasureText("Fire [space]", 40) / 2, GetScreenHeight() / 2 + 200, 40, (ui->beat_scale >= 1.04 || ui->offbeat_scale >= 1.04 ? BEAT_COLOR : WHITE));

    // Accelerate.
    DrawText("Accelerate [up arrow]", GetScreenWidth() / 2 - MeasureText("Accelerate [up arrow]", 40) / 2, GetScreenHeight() / 2 + 250, 40, (ui->beat_scale >= 1.04 || ui->offbeat_scale >= 1.04 ? BEAT_COLOR : WHITE));

    // Turn.
    DrawText("Turn left [left arrow]", GetScreenWidth() / 2 - MeasureText("Turn left [left arrow]", 40) / 2, GetScreenHeight() / 2 + 300, 40, (ui->beat_scale >= 1.04 || ui->offbeat_scale >= 1.04 ? BEAT_COLOR : WHITE));
    DrawText("Turn right [right arrow]", GetScreenWidth() / 2 - MeasureText("Turn right [right arrow]", 40) / 2, GetScreenHeight() / 2 + 350, 40, (ui->beat_scale >= 1.04 || ui->offbeat_scale >= 1.04 ? BEAT_COLOR : WHITE));
}


void in_game_ui(Game* game)
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
            (game->player.scale >= 1.08 ? (Color){ BEAT_COLOR.r - 50, BEAT_COLOR.g - 50, BEAT_COLOR.b - 50, 255 } : GRAY));

    text_offset_y += ui->score_size;

    // Draw the timer.
    DrawText(TextFormat("%ds", time(NULL) - game->start_time),
            GetMonitorWidth(0) / 2 - MeasureText(TextFormat("%ds", time(NULL) - game->start_time), ui->timer_size) / 2, 
            GetMonitorHeight(0) / 2 + text_offset_y, 
            ui->timer_size, 
            (game->player.scale >= 1.08 ? (Color){ BEAT_COLOR.r - 50, BEAT_COLOR.g - 50, BEAT_COLOR.b - 50, 255 } : GRAY));

    text_offset_y += ui->timer_size * 2.5;

    // Draw the lives.
    Vector2 triangle_offset = { (GetMonitorWidth(0) - ((game->player.hp - 1) * ui->life_size) - ((game->player.hp - 1) * 8)) / 2,
                                 GetMonitorHeight(0) / 2 + text_offset_y - 15 };

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
            for (int j = 0; j < 3; j++)
            {
                DrawLineEx(points[j], points[(j+1)%3], 2, (game->player.scale >= 1.08 ? (Color){ BEAT_COLOR.r - 50, BEAT_COLOR.g - 50, BEAT_COLOR.b - 50, 255 } : GRAY));
            }
        }
    }
}


void game_over_screen(Game* game)
{
    // Get the game's UI.
    Ui* ui = &game->ui;

    // Draw the game over text.
    DrawText("GAME OVER", 
             GetMonitorWidth(0) / 2 - MeasureText("GAME OVER", ui->game_over_size) / 2, 
             400, 
             ui->game_over_size, 
             RED);

    // Draw the final score.
    DrawText(TextFormat("Score: %d", game->score), 
             GetMonitorWidth(0) / 2 - MeasureText(TextFormat("Score: %d", game->score), ui->info_size) / 2, 
             GetMonitorHeight(0) / 2 - ui->game_over_size + 5, 
             ui->info_size, 
             (ui->beat_scale >= 1.04 || ui->offbeat_scale >= 1.04 ? BEAT_COLOR : WHITE));

    // Draw the game duration.
    DrawText(TextFormat("Duration: %ds", game->end_time - game->start_time), 
             GetMonitorWidth(0) / 2 - MeasureText(TextFormat("Duration: %ds", game->end_time - game->start_time), ui->info_size) / 2, 
             GetMonitorHeight(0) / 2 - ui->game_over_size + ui->info_size + 10, 
             ui->info_size, 
             (ui->beat_scale >= 1.04 || ui->offbeat_scale >= 1.04 ? BEAT_COLOR : WHITE));

    // Show highscores title.
    DrawText("Highscores:", 
             GetMonitorWidth(0) / 2 - MeasureText("Highscores:", game->ui.highscore_size) / 2, 
             GetMonitorHeight(0) / 2 - ui->game_over_size + ui->info_size * 3, 
             ui->highscore_size, 
             (ui->beat_scale >= 1.04 || ui->offbeat_scale >= 1.04 ? (Color){ BEAT_COLOR.r - 50, BEAT_COLOR.g - 50, BEAT_COLOR.b - 50, 255 } : GRAY));

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
                GetMonitorHeight(0) / 2 - ui->game_over_size + ui->info_size * 3 + (ui->highscore_size + 5) * (i+1),
                ui->highscore_size,
                (highscores[i][0] == game->score ? (ui->beat_scale >= 1.04 || ui->offbeat_scale >= 1.04 ? BEAT_COLOR : WHITE) : (ui->beat_scale >= 1.04 || ui->offbeat_scale >= 1.04 ? (Color){ BEAT_COLOR.r - 50, BEAT_COLOR.g - 50, BEAT_COLOR.b - 50, 255 } : GRAY)));
    }

    // Draw the restart promt.
    DrawText("Restart [space]", 
             GetMonitorWidth(0) / 2 - MeasureText("Restart [space]", ui->restart_size) / 2, 
             GetMonitorHeight(0) - ui->restart_size * 7, 
             ui->restart_size, 
             (ui->beat_scale >= 1.04 || ui->offbeat_scale >= 1.04 ? BEAT_COLOR : WHITE));
    DrawText("Main Menu [escape]", 
             GetMonitorWidth(0) / 2 - MeasureText("Main Menu [escape]", ui->restart_size) / 2, 
             GetMonitorHeight(0) - ui->restart_size * 5.5, 
             ui->restart_size, 
             (ui->beat_scale >= 1.04 || ui->offbeat_scale >= 1.04 ? BEAT_COLOR : WHITE));
}