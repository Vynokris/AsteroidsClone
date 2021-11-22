#include "bullet.h"



void bullet_init(Bullet* bullets)
{
    for (int i = 0; i < BULLET_MAX_AMOUNT; i++) {
        bullets[i].pos = Vector2Zero();
        bullets[i].rotation = 0;
        bullets[i].lifespan = 0;
        bullets[i].powerful = false;
    }
}

void bullet_update(Bullet* bullets, int* multiplier)
{
    for (int i = 0; i < BULLET_MAX_AMOUNT; i++) {
        if (bullets[i].lifespan > 0) {
            bullets[i].pos.x += cosf(bullets[i].rotation) * BULLET_SPEED;
            bullets[i].pos.y += sinf(bullets[i].rotation) * BULLET_SPEED;
            bullets[i].lifespan--;
        }
        if (bullets[i].lifespan == 1 && !bullets[i].powerful) {
            *multiplier = 1;
            bullets[i].lifespan = 0;
        }

        // Wrap around the screen.
        if (bullets[i].pos.x + 2 < 0) {
            bullets[i].pos.x = GetMonitorWidth(0) + 2;
        }
        else if (bullets[i].pos.x - 2 > GetMonitorWidth(0)) {
            bullets[i].pos.x = 0 - 2;
        }
        if (bullets[i].pos.y + 2 < 0) {
            bullets[i].pos.y = GetMonitorHeight(0) + 2;
        }
        else if (bullets[i].pos.y - 2 > GetMonitorHeight(0)) {
            bullets[i].pos.y = 0 - 2;
        }
    }
}


void bullet_draw(Bullet* bullets)
{
    for (int i = 0; i < BULLET_MAX_AMOUNT; i++) {
        if (bullets[i].lifespan > 0) {
            if (!bullets[i].powerful)
                DrawCircle(bullets[i].pos.x, bullets[i].pos.y, 2, WHITE);
            else
                DrawCircle(bullets[i].pos.x, bullets[i].pos.y, 2, BEAT_COLOR);
        }
    }
}