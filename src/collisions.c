#include "collisions.h"


bool player_collision(Player* player, Asteroid* asteroids, int* multiplier)
{
    // If the player isn't invulnerable.
    if (!player->invulnerable) {
        // For each non-destroyed asteroid.
        for (int i = 0; i < ASTEROID_MAX_AMOUNT; i++) {
            if (asteroids[i].type != A_DESTROYED) {
                // Check collision between it and the player.
                if (CheckCollisionCircles(player->pos, 40, asteroids[i].pos, asteroids[i].size)) {
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
        if (bullets[i].lifespan > 0) {
            // For each non-destroyed asteroid.
            for (int j = 0; j < ASTEROID_MAX_AMOUNT; j++) {
                if (asteroids[j].type != A_DESTROYED) {
                    // Check collision between the two.
                    if (CheckCollisionPointCircle(bullets[i].pos, asteroids[j].pos, asteroids[j].size)) {
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