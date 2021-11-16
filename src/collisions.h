#pragma once

#include "define.h"
#include "particle.h"
#include "player.h"
#include "bullet.h"
#include "asteroid.h"


// Checks if the player is colliding with an asteroid.
// If that is the case, decrements his hp and respawn him.
bool player_collision(Player* player, Asteroid* asteroids, int* multiplier);

// Check if any bullets are colliding with any asteroid. 
// If that is the case, destroy the bullet and break the asteroid.
bool bullet_collision(Bullet* bullets, Asteroid* asteroids, int* score, int* multiplier, Particle* asteroid_particles);