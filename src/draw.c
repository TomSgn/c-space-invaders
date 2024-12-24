#include "draw.h"
#include "globals.h"

void DrawStars(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    for (int i = 0; i < NUM_STARS; i++) {
        SDL_Rect starRect = {g_stars[i].x, g_stars[i].y, 2, 2};
        SDL_RenderFillRect(renderer, &starRect);
    }
}

void DrawBullets(SDL_Renderer* renderer) {
    for (int i = 0; i < NUM_BULLETS; i++) {
        if (g_bullets[i].active) {
            if (g_bullets[i].vx == 0.0f && g_bullets[i].targetX == 0.0f && g_bullets[i].targetY == 0.0f) {
                SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255); // Jaune
                SDL_Rect bulletRect = {(int)g_bullets[i].x, (int)g_bullets[i].y, 4, 12};
                SDL_RenderFillRect(renderer, &bulletRect);
            } else {
                SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255); // Magenta
                SDL_Rect bulletRect = {(int)g_bullets[i].x, (int)g_bullets[i].y, 4, 12};
                SDL_RenderFillRect(renderer, &bulletRect);
            }
        }
    }
}

void DrawEnemies(SDL_Renderer* renderer) {
    for (int i = 0; i < NUM_ENEMIES; i++) {
        if (g_enemies[i].active) {
            SDL_Rect enemyRect = {(int)g_enemies[i].x, (int)g_enemies[i].y, 34, 34};
            int textureIndex = i % 11;
            if (g_enemyTextures[textureIndex]) {
                SDL_RenderCopyEx(renderer, g_enemyTextures[textureIndex], NULL, &enemyRect, 180, NULL, SDL_FLIP_NONE);
            } else {
                SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
                SDL_RenderFillRect(renderer, &enemyRect);
            }
        }
    }
}

void DrawShip(SDL_Renderer* renderer) {
    if (g_shipTexture) {
        SDL_RenderCopy(renderer, g_shipTexture, NULL, &g_ship.rect);
    } else {
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
        SDL_RenderFillRect(renderer, &g_ship.rect);
    }
    if (g_shieldActive) {
        SDL_Color shieldColor = {0, 255, 0, 128};
        int centerX = g_ship.rect.x + g_ship.rect.w / 2;
        int centerY = g_ship.rect.y + g_ship.rect.h / 2;
        int radius = g_ship.rect.w / 2 + 10 + (int)(5 * sinf(g_shieldPulse)); // Ajoutez l'effet de respiration
        DrawFilledCircle(renderer, centerX, centerY, radius, shieldColor);
    }
}

void DrawLives(SDL_Renderer* renderer, int lives, int maxLives) {
    SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255);
    SDL_Rect bg = {10, 10, 100, 10};
    SDL_RenderFillRect(renderer, &bg);

    int w = (int)(100.0f * (float)lives / (float)maxLives);
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_Rect fg = {10, 10, w, 10};
    SDL_RenderFillRect(renderer, &fg);
}

void DrawScore(SDL_Renderer* renderer, TTF_Font* font, int score) {
    char scoreText[20];
    sprintf(scoreText, "Score: %d", score);

    SDL_Color white = {255, 255, 255, 255};
    SDL_Surface* surface = TTF_RenderText_Solid(font, scoreText, white);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_Rect rect = {10, 25, surface->w, surface->h};
    SDL_RenderCopy(renderer, texture, NULL, &rect);

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

void DrawMenu(SDL_Renderer* renderer, TTF_Font* font) {
    SDL_Color white = {255, 255, 255, 255};
    SDL_Surface* surface = TTF_RenderText_Solid(font, "Press Enter to Start", white);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_Rect rect = {
        WINDOW_WIDTH/2 - surface->w/2,
        WINDOW_HEIGHT/2 - surface->h/2,
        surface->w,
        surface->h
    };
    SDL_RenderCopy(renderer, texture, NULL, &rect);

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

void DrawGameOver(SDL_Renderer* renderer, TTF_Font* font) {
    SDL_Color white = {255, 255, 255, 255};
    SDL_Surface* surface = TTF_RenderText_Solid(font, "Game Over! Press Enter", white);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_Rect rect = {
        WINDOW_WIDTH/2 - surface->w/2,
        WINDOW_HEIGHT/2 - surface->h/2,
        surface->w,
        surface->h
    };
    SDL_RenderCopy(renderer, texture, NULL, &rect);

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

void DrawTrails(SDL_Renderer* renderer) {
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    for (int i = 0; i < NUM_TRAILS; i++) {
        if (g_trails[i].active) {
            float ratio = (float)g_trails[i].lifetime / TRAIL_MAX_LIFETIME;
            Uint8 alpha = (Uint8)(255 * ratio);
            int size = (int)(6 * ratio); // Ajuster pour un effet de taille décroissante

            SDL_SetRenderDrawColor(renderer, 
                                   g_trails[i].color.r, 
                                   g_trails[i].color.g, 
                                   g_trails[i].color.b, 
                                   alpha);

            SDL_Rect trailRect = {
                (int)g_trails[i].x, 
                (int)g_trails[i].y, 
                size, 
                size
            };
            SDL_RenderFillRect(renderer, &trailRect);
        }
    }
}

void DrawFilledCircle(SDL_Renderer* renderer, int x, int y, int radius, SDL_Color color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    for (int w = 0; w < radius * 2; w++) {
        for (int h = 0; h < radius * 2; h++) {
            int dx = radius - w; // horizontal offset
            int dy = radius - h; // vertical offset
            if ((dx*dx + dy*dy) <= (radius * radius)) {
                SDL_RenderDrawPoint(renderer, x + dx, y + dy);
            }
        }
    }
}
