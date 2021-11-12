#include "collisions.h"


bool player_collision(Player* player, Asteroid* asteroids, int* multiplier)
{
    // If the player isn't invulnerable.
    if (!player->invulnerable) 
    {
        // Create a shape for the player.
        ShapeInfo player_shape;
        player_shape.type = TRIANGLE;
        player_shape.data.triangle = TriangleCreate(Vector2Create(player->pos.x + sinf(player->rotation) * 20, player->pos.y - cosf(player->rotation) * 20),
                                                    Vector2Create(player->pos.x - sinf(player->rotation) * 20, player->pos.y + cosf(player->rotation) * 20),
                                                    Vector2Create(player->pos.x + cosf(player->rotation) * 40, player->pos.y + sinf(player->rotation) * 40));

        // For each non-destroyed asteroid.
        for (int i = 0; i < ASTEROID_MAX_AMOUNT; i++) {
            if (asteroids[i].type != A_DESTROYED) 
            {
                // Create a shape for the asteroid.
                ShapeInfo asteroid_shape;
                asteroid_shape.type = POLYGON;
                asteroid_shape.data.polygon = PolygonCreate(asteroids[i].pos, asteroids[i].size, asteroids[i].rotation, asteroids[i].sides);

                // Check collision between it and the player.
                if (collisionSAT(player_shape, asteroid_shape)) {
                    player_respawn(player);
                    *multiplier = 1;
                    player->hp--;
                    return true;
                }
            }
        }
    }
    return false;
}


bool bullet_collision(Bullet* bullets, Asteroid* asteroids, int* score, int* multiplier)
{
    bool collision = false;

    // For each active bullet.
    for (int i = 0; i < BULLET_MAX_AMOUNT; i++) {
        if (bullets[i].lifespan > 0) 
        {
            // Create a shape for the bullet.
            ShapeInfo bullet_shape;
            bullet_shape.type = CIRCLE;
            bullet_shape.data.circle = CircleCreate(bullets[i].pos, 2);

            // For each non-destroyed asteroid.
            for (int j = 0; j < ASTEROID_MAX_AMOUNT; j++) {
                if (asteroids[j].type != A_DESTROYED) 
                {
                    // Create a shape for the asteroid.
                    ShapeInfo asteroid_shape;
                    asteroid_shape.type = POLYGON;
                    asteroid_shape.data.polygon = PolygonCreate(asteroids[j].pos, asteroids[j].size, asteroids[j].rotation, asteroids[j].sides);

                    // Check collision between the two.
                    if (collisionSAT(bullet_shape, asteroid_shape)) {
                        asteroid_break(asteroids, j);
                        *score += (j+1) * (*multiplier);
                        (*multiplier)++;
                        bullets[i].lifespan = 0;
                        collision = true;
                        break;
                    }
                }
            }
        }
    }
    return collision;
}