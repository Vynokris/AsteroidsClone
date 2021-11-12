#include <stdlib.h>
#include <time.h>

#include "raylib.h"

#include "game.h"

int main(void)
{
    // ----- INITIALISATION ----- //

    srand(time(NULL));

    // Set the window size.
    const int screenWidth = 1920;
    const int screenHeight = 1080;


    // Create the window and make it fullscreen.
    InitWindow(screenWidth, screenHeight, "Asteroids clone");

    // Initialize the game structure.
    Game game;
    game_init(&game);

    // Set the window's fps.
    SetTargetFPS(60);

    // Create a RenderTexture2D to be used for render to texture.
    RenderTexture2D rendertexture = LoadRenderTexture(screenWidth, screenHeight);


    // ----- GAME LOOP ----- //

    while (!WindowShouldClose())
    {
        game_update(&game);

        debug_keys(&game);

        // Render the game objects on a texture.
        BeginTextureMode(rendertexture);
        {
            ClearBackground(BLACK);

            game_render(&game);
        }
        EndTextureMode();

        // Apply shaders to the texture and render it.
        BeginDrawing();
        {
            ClearBackground(BLACK);

            // The render texture must be y-flipped due to default OpenGL coordinates (left-bottom)
            DrawTextureRec(rendertexture.texture, (Rectangle){ 0, 0, screenWidth, -screenHeight }, (Vector2){ game.screen_offset[0], game.screen_offset[1] }, WHITE);
        }
        EndDrawing();
    }


    // ----- DE-INITIALISATION ----- //

    // Close window and OpenGL context.
    CloseWindow();

    return 0;
}