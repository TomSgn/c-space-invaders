#include "init.h"
#include "globals.h"
#include <SDL2/SDL_image.h> // Ajoutez cette ligne

void InitStars() {
    for (int i = 0; i < NUM_STARS; i++) {
        g_stars[i].x     = rand() % WINDOW_WIDTH;
        g_stars[i].y     = rand() % WINDOW_HEIGHT;
        g_stars[i].speed = 1.0f + (float)(rand() % 200) / 100.0f; 
    }
}

void InitBullets() {
    for (int i = 0; i < NUM_BULLETS; i++) {
        g_bullets[i].active = 0;
    }
}

void InitEnemies(SDL_Renderer* renderer) {
    for (int i = 0; i < NUM_ENEMIES; i++) {
        g_enemies[i].active = 0;
        g_enemies[i].vy = 1.0f + (float)(rand() % 200) / 100.0f; // Augmenter la vitesse des ennemis
    }
    char filename[40];
    for (int i = 0; i < 11; i++) {
        sprintf(filename, "assets/2X/tiny_ship%d.png", i + 1);
        g_enemyTextures[i] = IMG_LoadTexture(renderer, filename);
        if (!g_enemyTextures[i]) {
            printf("Avertissement: impossible de charger la texture de l'ennemi %d : %s\n", i + 1, IMG_GetError());
        }
    }
}

void InitTrails() {
    for (int i = 0; i < NUM_TRAILS; i++) {
        g_trails[i].active = 0;
    }
}
