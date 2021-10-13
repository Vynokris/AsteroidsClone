#pragma once

#include "player.h"
#include "bullet.h"
#include "asteroid.h"


// Checks if the player is colliding with an asteroid.
// If that is the case, decrements his hp and respawn him.
void player_collision(Player* player, Asteroid* asteroids, int* multiplier);

// Check if any bullets are colliding with any asteroid. 
// If that is the case, destroy the bullet and break the asteroid.
void bullet_collision(Bullet* bullets, Asteroid* asteroids, int* score, int* multiplier);