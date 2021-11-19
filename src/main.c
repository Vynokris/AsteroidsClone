#include <stdlib.h>
#include <time.h>

#include "raylib.h"

#include "game.h"

int main(void)
{
    // ----- INITIALISATION ----- //

    srand(time(NULL));

    // Create the window.
    InitWindow(1920, 1080, "Modern Asteroids");

    // Get the monitor resolution.
    const int screenWidth = GetMonitorWidth(0);
    const int screenHeight = GetMonitorHeight(0);

    // Change the window size and position to fit in the monitor.
    SetWindowPosition(0, 0);
    SetWindowSize(screenWidth, screenHeight);


    // Initialize the game structure.
    Game game;
    game_init(&game);

    // Set the window's fps.
    SetTargetFPS(FPS);


    // ----- GAME LOOP ----- //

    while (!WindowShouldClose())
    {
        game_update(&game);

        debug_keys(&game);

        // Renders the ui on the ui rendertexture.
        ui_render(&game);

        // Render the game objects on the game rendertexture.
        game_render(&game);

        // Render the two rendertextures on screen.
        BeginDrawing();
        {
            ClearBackground(BLACK);

            // The render textures must be y-flipped due to default OpenGL coordinates (left-bottom).
            DrawTexturePro(game.ui.rendertexture.texture, 
                          (Rectangle){ 0.0f, 0.0f, (float)game.ui.rendertexture.texture.width, (float)-game.ui.rendertexture.texture.height },
                          (Rectangle){ (GetScreenWidth()  - ((float)GetScreenWidth()  * (game.ui.offbeat_scale * game.ui.beat_scale))) * 0.5, 
                                       (GetScreenHeight() - ((float)GetScreenHeight() * (game.ui.offbeat_scale * game.ui.beat_scale))) * 0.5, 
                                                             (float)GetScreenWidth()  * (game.ui.offbeat_scale * game.ui.beat_scale), 
                                                             (float)GetScreenHeight() * (game.ui.offbeat_scale * game.ui.beat_scale) }, 
                          (Vector2){ 0, 0 }, 
                          0, 
                          WHITE);
            DrawTextureRec(game.   rendertexture.texture, (Rectangle){ 0, 0, screenWidth, -screenHeight }, (Vector2){ game.screen_offset[0], game.screen_offset[1] }, WHITE);

            //! Debug.
            DrawText(TextFormat("Current beat:    %d\nFrames till beat: %d", game.current_beat, game.frames_till_beat), 10, 10, 20, WHITE);
        }
        EndDrawing();
    }


    // ----- DE-INITIALISATION ----- //

    // Close window and OpenGL context.
    CloseWindow();

    return 0;
}