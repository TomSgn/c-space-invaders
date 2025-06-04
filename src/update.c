#include "update.h"
#include "globals.h"
#include <stdio.h> // Ajoutez cette ligne pour les impressions de débogage
#include <math.h> // Ajoutez cette ligne pour les fonctions mathématiques

float g_shieldPulse = 0.0f; // Ajoutez cette ligne

void UpdateStars() {
    for (int i = 0; i < NUM_STARS; i++) {
        g_stars[i].y += (int)g_stars[i].speed; 
        if (g_stars[i].y >= WINDOW_HEIGHT) {
            g_stars[i].y     = 0;
            g_stars[i].x     = rand() % WINDOW_WIDTH;
            g_stars[i].speed = 1.0f + (float)(rand() % 200) / 100.0f;
        }
    }
}

void UpdateBullets() {
    for (int i = 0; i < NUM_BULLETS; i++) {
        if (g_bullets[i].active) {
            g_bullets[i].x += g_bullets[i].vx;
            g_bullets[i].y += g_bullets[i].vy;
            g_bullets[i].hitbox.x = (int)g_bullets[i].x; // Ajoutez cette ligne
            g_bullets[i].hitbox.y = (int)g_bullets[i].y; // Ajoutez cette ligne
            if (g_bullets[i].vx != 0.0f || g_bullets[i].targetX != 0.0f || g_bullets[i].targetY != 0.0f) {
                Enemy* target = FindNearestEnemy(g_bullets[i].x, g_bullets[i].y);
                if (target) {
                    float dx = (target->x + target->hitbox.w / 2.0f) - g_bullets[i].x;
                    float dy = (target->y + target->hitbox.h / 2.0f) - g_bullets[i].y;
                    float length = sqrtf(dx * dx + dy * dy);
                    if (length != 0) {
                        g_bullets[i].vx = (dx / length) * 12.0f;
                        g_bullets[i].vy = (dy / length) * 12.0f;
                    }
                }
            }
            if (g_bullets[i].y < 0 || g_bullets[i].x < 0 || g_bullets[i].x > WINDOW_WIDTH || g_bullets[i].y > WINDOW_HEIGHT) {
                g_bullets[i].active = 0;
            }
        }
    }
}

void UpdateEnemies() {
    for (int i = 0; i < NUM_ENEMIES; i++) {
        if (g_enemies[i].active) {
            g_enemies[i].y += g_enemies[i].vy;
            g_enemies[i].hitbox.x = (int)g_enemies[i].x;
            g_enemies[i].hitbox.y = (int)g_enemies[i].y;
            if (g_enemies[i].y > WINDOW_HEIGHT) {
                g_enemies[i].active = 0;
            }
        }
    }
}

void UpdateTrails() {
    for (int i = 0; i < NUM_TRAILS; i++) {
        if (g_trails[i].active) {
            g_trails[i].x += g_trails[i].vx;
            g_trails[i].y += g_trails[i].vy;
            g_trails[i].lifetime -= 1;
            if (g_trails[i].lifetime <= 0 ||
                g_trails[i].x < 0 || g_trails[i].x >= WINDOW_WIDTH ||
                g_trails[i].y < 0 || g_trails[i].y >= WINDOW_HEIGHT) {
                g_trails[i].active = 0;
            } else {
                g_trails[i].color.a = (Uint8)(255 * g_trails[i].lifetime / 60);
            }
        }
    }
}

void CheckBulletEnemyCollision() {
    for (int i = 0; i < NUM_BULLETS; i++) {
        if (g_bullets[i].active) {
            for (int j = 0; j < NUM_ENEMIES; j++) {
                if (g_enemies[j].active && SDL_HasIntersection(&g_bullets[i].hitbox, &g_enemies[j].hitbox)) {
                    printf("Collision détectée entre une balle et un ennemi\n"); // Ajoutez cette ligne
                    g_bullets[i].active = 0;

                    // Créez des particules d'explosion à l'endroit de la collision
                    for (int k = 0; k < 10; k++) {
                        float angle = (float)k / 10.0f * 2.0f * M_PI;
                        float speed = 2.0f + (rand() % 100) / 50.0f;
                        SDL_Color color = {255, (Uint8)(rand() % 256), 0, 255}; // Jaune à rouge
                        CreateTrail(g_enemies[j].hitbox.x + g_enemies[j].hitbox.w / 2, g_enemies[j].hitbox.y + g_enemies[j].hitbox.h / 2, cosf(angle) * speed, sinf(angle) * speed, color);
                    }

                    g_enemies[j].active = 0;
                    g_score += 100; // Augmentez le score pour chaque ennemi détruit
                    break;
                }
            }
        }
    }
}

void CheckMissileEnemyCollision() {
    for (int i = 0; i < NUM_ENEMIES; i++) {
        if (g_enemies[i].active) {
            for (int j = 0; j < NUM_BULLETS; j++) {
                if (g_bullets[j].active && g_bullets[j].type == MISSILE) {
                    if (SDL_HasIntersection(&g_enemies[i].hitbox, &g_bullets[j].hitbox)) {
                        printf("Collision détectée entre un missile et un ennemi\n"); // Ajoutez cette ligne
                        g_bullets[j].active = 0;
                        g_enemies[i].active = 0; // Détruire l'ennemi immédiatement

                        // Créez des particules d'explosion à l'endroit de la collision
                        for (int k = 0; k < 10; k++) {
                            float angle = (float)k / 10.0f * 2.0f * M_PI;
                            float speed = 2.0f + (rand() % 100) / 50.0f;
                            SDL_Color color = {255, (Uint8)(rand() % 256), 0, 255}; // Jaune à rouge
                            CreateTrail(g_enemies[i].hitbox.x + g_enemies[i].hitbox.w / 2, g_enemies[i].hitbox.y + g_enemies[i].hitbox.h / 2, cosf(angle) * speed, sinf(angle) * speed, color);
                        }

                        g_score += 100; // Ajoutez des points pour chaque ennemi détruit
                    }
                }
            }
        }
    }
}

void CheckShootEnemyCollision() {
    for (int i = 0; i < NUM_ENEMIES; i++) {
        if (g_enemies[i].active) {
            for (int j = 0; j < NUM_BULLETS; j++) {
                if (g_bullets[j].active && g_bullets[j].type == BULLET) {
                    if (SDL_HasIntersection(&g_enemies[i].hitbox, &g_bullets[j].hitbox)) {
                        printf("Collision détectée entre un tir et un ennemi\n"); // Ajoutez cette ligne
                        g_enemies[i].active = 0;
                        g_bullets[j].active = 0;
                        g_score += 100; // Ajoutez des points pour chaque ennemi détruit
                    }
                }
            }
        }
    }
}

void CheckShieldEnemyCollision() {
    if (g_shieldActive) {
        SDL_Rect shieldRect = {
            g_ship.rect.x - 10,
            g_ship.rect.y - 10,
            g_ship.rect.w + 20,
            g_ship.rect.h + 20
        };
        for (int i = 0; i < NUM_ENEMIES; i++) {
            if (g_enemies[i].active && SDL_HasIntersection(&shieldRect, &g_enemies[i].hitbox)) {
                printf("Collision détectée entre le bouclier et un ennemi\n"); // Ajoutez cette ligne
                g_enemies[i].active = 0;
            }
        }
    }
}

void UpdateShield() {
    if (g_shieldActive) {
        g_shieldPulse += 0.1f; // Ajustez la vitesse de pulsation si nécessaire
        if (g_shieldPulse > 2 * M_PI) {
            g_shieldPulse -= 2 * M_PI;
        }
    }
}

void SpawnEnemy() {
    for (int i = 0; i < NUM_ENEMIES; i++) {
        if (!g_enemies[i].active) {
            g_enemies[i].x = rand() % (WINDOW_WIDTH - 34);
            g_enemies[i].y = -34;
            g_enemies[i].vy = 2.0f;
            g_enemies[i].hitbox = (SDL_Rect){(int)g_enemies[i].x, (int)g_enemies[i].y, 34, 34}; // Ajoutez cette ligne
            g_enemies[i].active = 1;
            break;
        }
    }
}

// Vérifie la collision entre le vaisseau et les ennemis (réduit les vies si sans bouclier)
void CheckShipEnemyCollision() {
    if (!g_shieldActive) {
        for (int i = 0; i < NUM_ENEMIES; i++) {
            if (g_enemies[i].active && SDL_HasIntersection(&g_ship.rect, &g_enemies[i].hitbox)) {
                printf("Collision détectée entre le vaisseau et un ennemi\n");
                // Crée des particules d'explosion à l'emplacement du vaisseau
                for (int k = 0; k < 10; k++) {
                    float angle = (float)k / 10.0f * 2.0f * M_PI;
                    float speed = 2.0f + (rand() % 100) / 50.0f;
                    SDL_Color color = {255, (Uint8)(rand() % 256), 0, 255};
                    CreateTrail(
                        g_ship.rect.x + g_ship.rect.w / 2,
                        g_ship.rect.y + g_ship.rect.h / 2,
                        cosf(angle) * speed,
                        sinf(angle) * speed,
                        color
                    );
                }
                g_enemies[i].active = 0;
                g_ship.lives--;
                break;
            }
        }
    }
}
