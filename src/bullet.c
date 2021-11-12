#include "bullet.h"



void bullet_init(Bullet* bullets)
{
    for (int i = 0; i < BULLET_MAX_AMOUNT; i++) {
        bullets[i].pos = Vector2Zero();
        bullets[i].rotation = 0;
        bullets[i].lifespan = 0;
    }
}

void bullet_update(Bullet* bullets, int* multiplier)
{
    for (int i = 0; i < BULLET_MAX_AMOUNT; i++) {
        if (bullets[i].lifespan > 0) {
            bullets[i].pos.x += cosf(bullets[i].rotation) * BULLET_SPEED;
            bullets[i].pos.y += sinf(bullets[i].rotation) * BULLET_SPEED;
            bullets[i].lifespan--;

            if (bullets[i].pos.x < 0 || bullets[i].pos.x > GetMonitorWidth(0) || bullets[i].pos.y < 0 || bullets[i].pos.y > GetMonitorHeight(0)) {
                bullets[i].lifespan = 0;
                *multiplier = 1;
            }
        }
    }
}


void bullet_draw(Bullet* bullets)
{
    for (int i = 0; i < BULLET_MAX_AMOUNT; i++) {
        if (bullets[i].lifespan > 0) {
            DrawCircle(bullets[i].pos.x, bullets[i].pos.y, 2, WHITE);
        }
    }
}