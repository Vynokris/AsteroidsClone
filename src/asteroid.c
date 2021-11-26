#include "asteroid.h"



void asteroid_init(Asteroid* asteroids)
{
    for (int i = 0; i < ASTEROID_MAX_AMOUNT; i++) 
    {
        // Position.
        asteroids[i].pos = Vector2Zero();
        // Velocity.
        asteroids[i].velocity = Vector2Zero();
        // Rotation.
        asteroids[i].rotation = 0.0f;
        asteroids[i].rotation_speed = 0.0f;
        // Type.
        asteroids[i].type = A_DESTROYED;
        // Size.
        asteroids[i].size = 0;
        // Scale.
        asteroids[i].scale = 1;
        // Side count.
        asteroids[i].sides = 0;
    }
}


void spawn_asteroid(Asteroid* asteroids)
{
    // ----- FIND ASTEROID ----- //
    int a_spwn = 0;
    for (int i = 0; i < ASTEROID_MAX_AMOUNT; i++) {
        if (asteroids[i].type == A_SMALL && a_spwn == 0) {
            a_spwn = i;
        }
        if (asteroids[i].type == A_DESTROYED) {
            a_spwn = i;
            break;
        }
    }

    // ----- TYPE ----- //
    asteroids[a_spwn].type = GetRandomValue(2, 3);


    // ----- SIZE & NUMBER OF SIDES ----- //
    asteroids[a_spwn].size  = GetRandomValue(ASTEROID_SIZE(asteroids[a_spwn].type)  - 5, ASTEROID_SIZE(asteroids[a_spwn].type) + 5);
    asteroids[a_spwn].sides = GetRandomValue(ASTEROID_SIDES(asteroids[a_spwn].type) - 1, ASTEROID_SIDES(asteroids[a_spwn].type));


    // ----- STARTING POSITION ----- //
    // 50% chance to spawn on the top or bottom of the screen.
    if (GetRandomValue(0, 1)) 
    {
        asteroids[a_spwn].pos.x = GetRandomValue(0, GetMonitorWidth(0));
        // 50% chance to spawn in the top.
        if (GetRandomValue(0, 1)) {
            asteroids[a_spwn].pos.y = 0 - asteroids[a_spwn].size;
        }
        // 50 % chance to spawn in the bottom.
        else {
            asteroids[a_spwn].pos.y = GetMonitorHeight(0) + asteroids[a_spwn].size;
        }
    }

    // 50% chance to spawn on the right or left of the screen.
    else 
    {
        asteroids[a_spwn].pos.y = GetRandomValue(0, GetMonitorHeight(0));
        // 50% chance to spawn on the left.
        if (GetRandomValue(0, 1)) {
            asteroids[a_spwn].pos.x = 0 - asteroids[a_spwn].size;
        }
        // 50 % chance to spawn on the right.
        else {
            asteroids[a_spwn].pos.x = GetMonitorWidth(0) + asteroids[a_spwn].size;
        }
    }


    // ----- VELOCITY ----- //
    // Get a random point on screen for the asteroid to go towards.
    MyVector2 rand_point = Vector2Create(GetRandomValue(GetMonitorWidth(0)  * 0.3, GetMonitorWidth(0)  * 0.7), 
                                         GetRandomValue(GetMonitorHeight(0) * 0.3, GetMonitorHeight(0) * 0.7));

    // Get a vector from the asteroid's position to the random point.
    MyVector2 rand_vector = Vector2Create(rand_point.x - asteroids[a_spwn].pos.x, rand_point.y - asteroids[a_spwn].pos.y);

    // Normalize the vector.
    {
        // Get the magnitude of the velocity vector.
        int magnitude = sqrt(pow(rand_vector.x, 2) + pow(rand_vector.y, 2));

        // Divide its values by its magnitude.
        rand_vector.x /= magnitude;
        rand_vector.y /= magnitude;
    }

    // Set the asteroid's velocity.
    asteroids[a_spwn].velocity = rand_vector;


    // ----- ROTATION ----- //
    asteroids[a_spwn].rotation = degToRad(GetRandomValue(0, 360));
    asteroids[a_spwn].rotation_speed = degToRad((float)GetRandomValue(1, 10) / 10);
}


void asteroid_update(Asteroid* asteroids)
{
    // Update the player's scale to the beat.
    double beat_scale = get_beat_scale_ms(1.15, 1, 0);

    for (int i = 0; i < ASTEROID_MAX_AMOUNT; i++) {
        if (asteroids[i].type != A_DESTROYED) 
        {
            // ----- MOVEMENT & ROTATION & SCALE ----- //
            asteroids[i].pos.x += asteroids[i].velocity.x * ASTEROID_SPEED(asteroids[i].type);
            asteroids[i].pos.y += asteroids[i].velocity.y * ASTEROID_SPEED(asteroids[i].type);
            asteroids[i].rotation += asteroids[i].rotation_speed;
            asteroids[i].scale = beat_scale;
            /*
            if (beat_scale >= 1.14)
                asteroids[i].rotation += PI / 40;
            */

            // ----- SCREEN WRAPPING ----- //
            if (asteroids[i].pos.x + asteroids[i].size + 5 < 0) {
                asteroids[i].pos.x = GetMonitorWidth(0) + asteroids[i].size;
            }
            else if (asteroids[i].pos.x - asteroids[i].size - 5 > GetMonitorWidth(0)) {
                asteroids[i].pos.x = 0 - asteroids[i].size;
            }
            if (asteroids[i].pos.y + asteroids[i].size + 5 < 0) {
                asteroids[i].pos.y = GetMonitorHeight(0) + asteroids[i].size;
            }
            else if (asteroids[i].pos.y - asteroids[i].size - 5 > GetMonitorHeight(0)) {
                asteroids[i].pos.y = 0 - asteroids[i].size;
            }
        }
    }
}


void asteroid_break(Asteroid* asteroids, int break_i)
{
    AsteroidType new_type = asteroids[break_i].type - 1;

    if (new_type != A_DESTROYED)
    {
        // Find another index to put the second part of the broken asteroid.
        int part_i = 0;
        for (int i = 0; i < ASTEROID_MAX_AMOUNT; i++) {
            if (asteroids[i].type == A_DESTROYED) {
                part_i = i;
                break;
            }
        }

        // Set the type.
        asteroids[part_i].type =  new_type;
        asteroids[break_i].type = new_type;
        // Set the position.
        asteroids[part_i].pos = asteroids[break_i].pos;
        // Set the size.
        asteroids[part_i].size =  GetRandomValue(ASTEROID_SIZE(new_type) - 5, ASTEROID_SIZE(new_type) + 5);
        asteroids[break_i].size = GetRandomValue(ASTEROID_SIZE(new_type) - 5, ASTEROID_SIZE(new_type) + 5);
        // Set the side count.
        asteroids[part_i].sides =  GetRandomValue(ASTEROID_SIDES(new_type) - 1, ASTEROID_SIDES(new_type));
        asteroids[break_i].sides = GetRandomValue(ASTEROID_SIDES(new_type) - 1, ASTEROID_SIDES(new_type));
        // Set the rotation.
        asteroids[part_i].rotation =  degToRad(GetRandomValue(0, 360));
        asteroids[break_i].rotation = degToRad(GetRandomValue(0, 360));
        // Set the rotation speed.
        asteroids[part_i].rotation_speed =  degToRad((float)GetRandomValue(1, 10) / 10);
        asteroids[break_i].rotation_speed = degToRad((float)GetRandomValue(1, 10) / 10);
        // Set the velocity as orthogonal to the velocity of the broken asteroid. The velocity of both parts is be opposed.
        asteroids[break_i].velocity = Vector2Rotate(asteroids[break_i].velocity, 90);
        asteroids[part_i].velocity =  Vector2Negate(asteroids[break_i].velocity);
    }
    else {
        asteroids[break_i].type = new_type;
    }
}


void asteroid_draw(Asteroid* asteroids)
{
    for (int i = 0; i < ASTEROID_MAX_AMOUNT; i++) {
        if (asteroids[i].type != A_DESTROYED) {
            // Sraw the asteroid interior.
            DrawPoly(toRayVec(asteroids[i].pos), asteroids[i].sides, asteroids[i].size * asteroids[i].scale, radToDeg(asteroids[i].rotation), BLACK);
            // Draw the asteroid exterior lines.
            DrawPolyLines(toRayVec(asteroids[i].pos), asteroids[i].sides, asteroids[i].size * asteroids[i].scale, radToDeg(asteroids[i].rotation),  (asteroids[i].scale > 1.08 ? BEAT_COLOR : WHITE));
            // Draw the asteroid number on it.
            DrawText(TextFormat("%d", i+1), 
                     asteroids[i].pos.x - MeasureText(TextFormat("%d", i+1), 20) / 2, 
                     asteroids[i].pos.y - MeasureText(TextFormat("%d", i+1), 20) / 2, 
                     20 * asteroids[i].scale, 
                     (asteroids[i].scale >= 1.08 ? BEAT_COLOR : WHITE));
        }
    }
}