#include "player.h"


void player_init(Player* player)
{
    player->hp = 3;
    player->invulnerable = 60 * 3;
    player->rotation = -PI / 2;
    player->pos = Vector2Create(GetMonitorWidth(0) / 2, GetMonitorHeight(0) / 2);
    player->velocity = Vector2Zero();
    particle_init(player->particles);
}


void player_shoot(Player* player, Bullet* bullets)
{
    if (player->hp > 0) {
        // Add a bullet if there are some bullets that still haven't been fired.
        for (int i = 0; i < BULLET_MAX_AMOUNT; i++) {
            if (bullets[i].lifespan == 0) {
                bullets[i].pos.x = player->pos.x + cosf(player->rotation) * 40;
                bullets[i].pos.y = player->pos.y + sinf(player->rotation) * 40;
                bullets[i].rotation = player->rotation;
                bullets[i].lifespan = BULLET_LIFESPAN;
                break;
            }
        }
    }
}


void player_update(Player* player, Bullet* bullets)
{
    // Add velocity if the player is pressing the up arrow.
    if (IsKeyDown(KEY_UP)) {
        // If the player isn't at max velocity, increment its velocity according to his rotation.
        if (sqrt(pow(player->velocity.x, 2) + pow(player->velocity.y, 2)) < PLAYER_MAX_VELOCITY) {
            player->velocity.x += cos(player->rotation) * PLAYER_SPEED;
            player->velocity.y += sin(player->rotation) * PLAYER_SPEED;
        }
        // If the player is at max velocity, decrement his velocity and increment it according to his rotation.
        else {
            player->velocity.x *= sqrt(pow(cos(player->rotation) * PLAYER_SPEED, 2) + pow(sin(player->rotation) * PLAYER_SPEED, 2));
            player->velocity.y *= sqrt(pow(cos(player->rotation) * PLAYER_SPEED, 2) + pow(sin(player->rotation) * PLAYER_SPEED, 2));
        }

        // Spawn particles behind him, only if he is visible.
        if ((player->invulnerable / 10) % 2 == 0) {
            particle_spawn(player->particles, 
                           Vector2Add(player->pos, Vector2FromAngle(player->rotation + (GetRandomValue(0, 1) ? PI/2 : -PI/2), 
                                                                    GetRandomValue(0, 10))), 
                           Vector2FromAngle(degToRad(radToDeg(player->rotation - 180) + GetRandomValue(-90, 90)), 1),
                           30,
                           5,
                           2,
                           15,
                           PARTICLE_LINES_FILLED,
                           (Vector3){ 200, 200, 200 });
        }
    }

    // Accelerate with the left joystick.
    else if (IsGamepadAvailable(0) && (fabs(GetGamepadAxisMovement(0, 0)) > 0.1 ||
                                       fabs(GetGamepadAxisMovement(0, 1)) > 0.1))
    {
        // Get the direction of the joystick.
        MyVector2 dir = Vector2Create(GetGamepadAxisMovement(0, 0), GetGamepadAxisMovement(0, 1));

        // If the player is under the maximum velocity, make him accelerate.
        if (Vector2Length(player->velocity) < PLAYER_MAX_VELOCITY)
            player->velocity = Vector2Add(                   player->velocity,       Vector2MultiplyVal(Vector2Normalize(dir), 0.6));

        // If the player is at maximum velocity, stop accelerating.
        else
            player->velocity = Vector2Add(Vector2MultiplyVal(player->velocity, 0.7), Vector2MultiplyVal(Vector2Normalize(dir), 0.3));
    }

    // Slow down if the player isn't pressing the up arrow.
    else {
        if (sqrt(pow(player->velocity.x, 2) + pow(player->velocity.y, 2)) > 0) {
            player->velocity.x *= 0.99;
            player->velocity.y *= 0.99;
        }
    }

    // Turn.
    if (IsKeyDown(KEY_LEFT)) {
        player->rotation -= PI / 48;
    }
    if (IsKeyDown(KEY_RIGHT)) {
        player->rotation += PI / 48;
    }

    // Rotate according to the right gamepad joystick.
    if (IsGamepadAvailable(0) && (fabs(GetGamepadAxisMovement(0, 2)) > 0.1 || 
                                  fabs(GetGamepadAxisMovement(0, 3)) > 0.1))
    {
        player->rotation = Vector2GetAngle(Vector2Normalize(Vector2Create(GetGamepadAxisMovement(0, 2), GetGamepadAxisMovement(0, 3))));
    }

    // Shoot.
    if (IsKeyPressed(KEY_SPACE) ||
       (IsGamepadAvailable(0) && IsGamepadButtonPressed(0, 12))) {
        player_shoot(player, bullets);
    }

    // Move according to velocity.
    player->pos.x += player->velocity.x;
    player->pos.y += player->velocity.y;

    // Wrap around the screen.
    if (player->pos.x + 10 < 0) {
        player->pos.x = GetMonitorWidth(0) + 10;
    }
    else if (player->pos.x - 10 > GetMonitorWidth(0)) {
        player->pos.x = 0 - 10;
    }
    if (player->pos.y + 10 < 0) {
        player->pos.y = GetMonitorHeight(0) + 10;
    }
    else if (player->pos.y - 10 > GetMonitorHeight(0)) {
        player->pos.y = 0 - 10;
    }

    // Decrement the player's invulnerability.
    if (player->invulnerable) {
        player->invulnerable--;
    }
}


void player_invulnerability(Player* player)
{
    // Give the player 3 seconds of invulnerability.
    player->invulnerable = 60 * 3;
}


void player_respawn(Player* player)
{
    // Put the player in the middle of the screen.
    player->pos.x = GetMonitorWidth(0) / 2;
    player->pos.y = GetMonitorHeight(0) / 2;

    // Set the player's velocity to 0.
    player->velocity = Vector2Zero();

    // Set the player's rotation to up.
    player->rotation = -PI/2;

    // Make the player invulnerable.
    player_invulnerability(player);
}


void player_draw(Player* player)
{
    if ((player->invulnerable / 10) % 2 == 0) 
    {
        Vector2 points[3] = 
        {
            { player->pos.x + sin(player->rotation) * 20, player->pos.y - cos(player->rotation) * 20 },
            { player->pos.x - sin(player->rotation) * 20, player->pos.y + cos(player->rotation) * 20 },
            { player->pos.x + cos(player->rotation) * 40, player->pos.y + sin(player->rotation) * 40 },
        };
        DrawTriangle(points[0], points[1], points[2], BLACK);
        DrawTriangleLines(points[0], points[1], points[2], WHITE);
    }
}