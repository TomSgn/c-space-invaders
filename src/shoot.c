#include "shoot.h"
#include "globals.h"

void ShootBullet() {
    for (int i = 0; i < NUM_BULLETS; i++) {
        if (!g_bullets[i].active) {
            g_bullets[i].x      = g_ship.rect.x + g_ship.rect.w / 2 - 2;
            g_bullets[i].y      = g_ship.rect.y;
            g_bullets[i].vy     = -8.0f;
            g_bullets[i].vx     = 0.0f;
            g_bullets[i].targetX = 0.0f;
            g_bullets[i].targetY = 0.0f;
            g_bullets[i].hitbox = (SDL_Rect){(int)g_bullets[i].x, (int)g_bullets[i].y, 4, 12};
            g_bullets[i].active = 1;
            g_bullets[i].type   = BULLET; // Ajoutez cette ligne
            break;
        }
    }
}

void ShootMissile(float offsetX) {
    for (int i = 0; i < NUM_BULLETS; i++) {
        if (!g_bullets[i].active) {
            g_bullets[i].x      = g_ship.rect.x + g_ship.rect.w / 2 - 1 + offsetX;
            g_bullets[i].y      = g_ship.rect.y;
            g_bullets[i].vy     = -12.0f;
            Enemy* target = FindNearestEnemy(g_bullets[i].x, g_bullets[i].y);
            if (target) {
                g_bullets[i].targetX = target->x + target->hitbox.w / 2.0f;
                g_bullets[i].targetY = target->y + target->hitbox.h / 2.0f;
                float dx = g_bullets[i].targetX - g_bullets[i].x;
                float dy = g_bullets[i].targetY - g_bullets[i].y;
                float length = sqrtf(dx * dx + dy * dy);
                if (length != 0) {
                    g_bullets[i].vx = (dx / length) * 5.0f;
                } else {
                    g_bullets[i].vx = 0;
                }
            } else {
                g_bullets[i].vx = 0;
            }
            g_bullets[i].active = 1;
            g_bullets[i].type   = MISSILE; // Ajoutez cette ligne
            break;
        }
    }
}

Enemy* FindNearestEnemy(float x, float y) {
    Enemy* nearestEnemy = NULL;
    float nearestDistance = FLT_MAX;
    for (int i = 0; i < NUM_ENEMIES; i++) {
        if (g_enemies[i].active) {
            float dx = g_enemies[i].x - x;
            float dy = g_enemies[i].y - y;
            float distance = sqrtf(dx * dx + dy * dy);
            if (distance < nearestDistance) {
                nearestDistance = distance;
                nearestEnemy = &g_enemies[i];
            }
        }
    }
    return nearestEnemy;
}
