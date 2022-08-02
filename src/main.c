#include <stdlib.h>
#include <time.h>

#include "raylib.h"

#include "define.h"
#include "game.h"

Game game;

void UpdateAndDrawFrame()
{
    // Initialize the game structure.
    bool setupDone = false;

    // Render the two rendertextures on screen.
    BeginDrawing();
    {
        ClearBackground(BLACK);

        // Updates the game objects.
        game_update(&game);
        
        // Checks if the debug keys were pressed.
        debug_keys(&game);

        // Renders the ui on the ui rendertexture.
        ui_render(&game);

        // Render the game objects on the game rendertexture.
        game_render(&game);

        // The render textures must be y-flipped due to default OpenGL coordinates (left-bottom).
        DrawTexturePro(game.ui.rendertexture.texture, 
                       (Rectangle){ 0.0f, 0.0f, (float)game.ui.rendertexture.texture.width, (float)-game.ui.rendertexture.texture.height },
                       (Rectangle){ (SCALED_SCREEN_WIDTH  - ((float)SCALED_SCREEN_WIDTH  * (game.ui.offbeat_scale * game.ui.beat_scale))) * 0.5, 
                                    (SCALED_SCREEN_HEIGHT - ((float)SCALED_SCREEN_HEIGHT * (game.ui.offbeat_scale * game.ui.beat_scale))) * 0.5, 
                                                             (float)SCALED_SCREEN_WIDTH  * (game.ui.offbeat_scale * game.ui.beat_scale), 
                                                             (float)SCALED_SCREEN_HEIGHT * (game.ui.offbeat_scale * game.ui.beat_scale) }, 
                       (Vector2){ 0, 0 }, 0, WHITE);
        DrawTexturePro(game.rendertexture.texture, 
                       (Rectangle){ 0, 0, SCREEN_WIDTH, -SCREEN_HEIGHT }, 
                       (Rectangle){ 0, 0, SCALED_SCREEN_WIDTH, SCALED_SCREEN_HEIGHT }, 
                       (Vector2)  { game.screen_offset[0], game.screen_offset[1] }, 0, WHITE);
    }
    EndDrawing();
}

int main(void)
{
    // ----- INITIALISATION ----- //

    // Create the window.
    InitWindow(SCALED_SCREEN_WIDTH, SCALED_SCREEN_HEIGHT, "Modern Asteroids");
    SetWindowPosition(0, 25);
    SetTargetFPS(FPS);

    srand(time(NULL));
    game_init(&game, false);

    /*
    // Reset the highscores.
    FILE* f = fopen("player_data/highscores.bin", "wb");
    int highscores[5][2] = { {0,0}, {0,0}, {0,0}, {0,0}, {0,0} };
    fwrite(highscores, sizeof(int) * 2, 5, f);
    fclose(f);
    */

    // ----- GAME LOOP ----- //
    #if defined(PLATFORM_WEB)
        emscripten_set_main_loop(UpdateAndDrawFrame, 0, 1);
    #else
        while (!WindowShouldClose())
            UpdateAndDrawFrame();
    #endif

    // Close window and OpenGL context.
    CloseWindow();

    return 0;
}