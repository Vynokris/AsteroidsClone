#include <stdlib.h>
#include <time.h>

#include "raylib.h"

#include "game.h"

int main(void)
{
    // ----- INITIALISATION ----- //

    srand(time(NULL));

    // Set the window size.
    const int screenWidth = GetScreenWidth();
    const int screenHeight = GetScreenHeight();

    // Create the window and make it fullscreen.
    InitWindow(screenWidth, screenHeight, "Asteroids clone");

    // Initialize the game structure.
    Game game;
    game_init(&game);

    // Set the window's fps.
    SetTargetFPS(60);


    // ----- GAME LOOP ----- //

    while (!WindowShouldClose())
    {
        game_update(&game);

        debug_keys(&game);

        BeginDrawing();
        {
            ClearBackground(BLACK);

            game_render(&game);
        }
        EndDrawing();
    }


    // ----- DE-INITIALISATION ----- //

    // Close window and OpenGL context.
    CloseWindow();

    return 0;
}