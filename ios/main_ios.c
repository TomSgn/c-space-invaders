#include "../include/init.h"
#include "../include/update.h"
#include "../include/draw.h"
#include "../include/shoot.h"
#include "../include/globals.h"
#if TARGET_OS_IPHONE
#include <SDL_image.h>
#else
#include <SDL2/SDL_image.h>
#endif
#include <stdio.h>

//-------------------------------
// CONFIG & CONSTANTES
//-------------------------------
#define WINDOW_WIDTH          375
#define WINDOW_HEIGHT         667
#define NUM_STARS             70
#define NUM_BULLETS           50
#define NUM_ENEMIES           400
#define FRAME_DELAY_MS        16
#define NUM_TRAILS            100
#define SHIP_SPEED            5
#define SHIP_MAX_LIVES        3

// Touch control areas
#define TOUCH_BUTTON_SIZE     80
#define TOUCH_MARGIN          20

//-------------------------------
// FONCTION PRINCIPALE
//-------------------------------
void CheckBulletEnemyCollision();
void CheckShieldEnemyCollision();
void CheckMissileEnemyCollision();
void CheckShootEnemyCollision();
void CheckShipEnemyCollision();
Enemy* FindNearestEnemy(float x, float y);
void SpawnEnemy();

// Déclarations des variables globales
GameState  g_gameState  = MENU;
Ship       g_ship       = {{WINDOW_WIDTH/2 - 17, WINDOW_HEIGHT - 50, 34, 34}, SHIP_MAX_LIVES, 0.0f, 0.0f};
Star       g_stars[NUM_STARS];
Bullet     g_bullets[NUM_BULLETS];
Enemy      g_enemies[NUM_ENEMIES];
TrailParticle g_trails[NUM_TRAILS];
int  g_score   = 0;
int g_shieldActive = 0;
SDL_Texture* g_shipTexture   = NULL;
SDL_Texture* g_enemyTextures[11] = {NULL};
TTF_Font*    g_font          = NULL;

void CreateTrail(float x, float y, float vx, float vy, SDL_Color color) {
    for (int i = 0; i < NUM_TRAILS; i++) {
        if (!g_trails[i].active) {
            g_trails[i].x = x;
            g_trails[i].y = y;
            g_trails[i].vx = vx;
            g_trails[i].vy = vy;
            g_trails[i].color = color;
            g_trails[i].lifetime = TRAIL_MAX_LIFETIME;
            g_trails[i].active = 1;
            break;
        }
    }
}

// Structure for touch controls
typedef struct {
    SDL_Rect rect;
    int active;
} TouchButton;

TouchButton shootButton;
TouchButton shieldButton;
TouchButton missileButton;

void InitTouchButtons() {
    // Shoot button (bottom right)
    shootButton.rect.x = WINDOW_WIDTH - TOUCH_BUTTON_SIZE - TOUCH_MARGIN;
    shootButton.rect.y = WINDOW_HEIGHT - TOUCH_BUTTON_SIZE - TOUCH_MARGIN;
    shootButton.rect.w = TOUCH_BUTTON_SIZE;
    shootButton.rect.h = TOUCH_BUTTON_SIZE;
    shootButton.active = 1;
    
    // Shield button (bottom left)
    shieldButton.rect.x = TOUCH_MARGIN;
    shieldButton.rect.y = WINDOW_HEIGHT - TOUCH_BUTTON_SIZE - TOUCH_MARGIN;
    shieldButton.rect.w = TOUCH_BUTTON_SIZE;
    shieldButton.rect.h = TOUCH_BUTTON_SIZE;
    shieldButton.active = 1;
    
    // Missile button (bottom center)
    missileButton.rect.x = WINDOW_WIDTH / 2 - TOUCH_BUTTON_SIZE / 2;
    missileButton.rect.y = WINDOW_HEIGHT - TOUCH_BUTTON_SIZE - TOUCH_MARGIN;
    missileButton.rect.w = TOUCH_BUTTON_SIZE;
    missileButton.rect.h = TOUCH_BUTTON_SIZE;
    missileButton.active = 1;
}

void DrawTouchButtons(SDL_Renderer* renderer) {
    if (g_gameState != GAME) return;
    
    // Draw shoot button
    SDL_SetRenderDrawColor(renderer, 255, 100, 100, 150);
    SDL_RenderFillRect(renderer, &shootButton.rect);
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderDrawRect(renderer, &shootButton.rect);
    
    // Draw shield button
    if (g_shieldActive) {
        SDL_SetRenderDrawColor(renderer, 100, 255, 100, 150);
    } else {
        SDL_SetRenderDrawColor(renderer, 100, 100, 255, 150);
    }
    SDL_RenderFillRect(renderer, &shieldButton.rect);
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    SDL_RenderDrawRect(renderer, &shieldButton.rect);
    
    // Draw missile button
    SDL_SetRenderDrawColor(renderer, 200, 100, 200, 150);
    SDL_RenderFillRect(renderer, &missileButton.rect);
    SDL_SetRenderDrawColor(renderer, 128, 0, 128, 255);
    SDL_RenderDrawRect(renderer, &missileButton.rect);
}

int HandleTouchButton(int x, int y, TouchButton* button) {
    if (x >= button->rect.x && x <= button->rect.x + button->rect.w &&
        y >= button->rect.y && y <= button->rect.y + button->rect.h) {
        return 1;
    }
    return 0;
}

int main(int argc, char* argv[]) {
    srand((unsigned int)time(NULL));

    // Init SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Erreur: SDL_Init: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }
    if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG) {
        printf("Erreur: IMG_Init: %s\n", IMG_GetError());
        SDL_Quit();
        return EXIT_FAILURE;
    }
    if (TTF_Init() == -1) {
        printf("Erreur: TTF_Init: %s\n", TTF_GetError());
        IMG_Quit();
        SDL_Quit();
        return EXIT_FAILURE;
    }

    // Créer la fenêtre (fullscreen sur iOS)
    SDL_Window* window = SDL_CreateWindow("Space Invaders",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH, WINDOW_HEIGHT,
        SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI);
    if (!window) {
        printf("Erreur: SDL_CreateWindow: %s\n", SDL_GetError());
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
        return EXIT_FAILURE;
    }

    // Créer le renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        printf("Erreur: SDL_CreateRenderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
        return EXIT_FAILURE;
    }

    // Activer le mélange alpha
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    // Charger une texture de vaisseau
    g_shipTexture = IMG_LoadTexture(renderer, "assets/2X/tiny_ship17.png");
    if (!g_shipTexture) {
        printf("Avertissement: impossible de charger la texture du vaisseau : %s\n", IMG_GetError());
    }

    // Charger la police
    g_font = TTF_OpenFont("assets/arial.ttf", 16);
    if (!g_font) {
        printf("Avertissement: TTF_OpenFont: %s\n", TTF_GetError());
    }

    // Init tout
    InitStars();
    InitBullets();
    InitEnemies(renderer);
    InitTrails();
    InitTouchButtons();

    // Variables de boucle
    int running = 1;
    Uint32 lastSpawn = 0;
    static int spawnDelay = 2000;
    
    // Touch tracking
    int touchMoving = 0;
    int touchX = 0, touchY = 0;

    while (running) {
        // EVENTS
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            }
            else if (event.type == SDL_FINGERDOWN || event.type == SDL_MOUSEBUTTONDOWN) {
                int x, y;
                if (event.type == SDL_FINGERDOWN) {
                    x = (int)(event.tfinger.x * WINDOW_WIDTH);
                    y = (int)(event.tfinger.y * WINDOW_HEIGHT);
                } else {
                    x = event.button.x;
                    y = event.button.y;
                }
                
                // Menu/Game Over - tap to start
                if (g_gameState == MENU || g_gameState == GAME_OVER) {
                    g_gameState = GAME;
                    g_ship.lives = SHIP_MAX_LIVES;
                    g_score = 0;
                    g_ship.rect.x = WINDOW_WIDTH / 2 - g_ship.rect.w / 2;
                    g_ship.rect.y = WINDOW_HEIGHT - 50;
                }
                // Game - check button presses
                else if (g_gameState == GAME) {
                    if (HandleTouchButton(x, y, &shootButton)) {
                        ShootBullet();
                    } else if (HandleTouchButton(x, y, &shieldButton)) {
                        g_shieldActive = !g_shieldActive;
                    } else if (HandleTouchButton(x, y, &missileButton)) {
                        ShootMissile(-10);
                        ShootMissile(10);
                        CreateTrail(
                            g_ship.rect.x + g_ship.rect.w / 2,
                            g_ship.rect.y + g_ship.rect.h / 2,
                            0.0f, 1.0f,
                            (SDL_Color){128, 0, 128, 255}
                        );
                    } else {
                        // Start tracking for ship movement
                        touchMoving = 1;
                        touchX = x;
                        touchY = y;
                    }
                }
            }
            else if (event.type == SDL_FINGERMOTION || event.type == SDL_MOUSEMOTION) {
                if (touchMoving && g_gameState == GAME) {
                    if (event.type == SDL_FINGERMOTION) {
                        touchX = (int)(event.tfinger.x * WINDOW_WIDTH);
                        touchY = (int)(event.tfinger.y * WINDOW_HEIGHT);
                    } else if (event.motion.state & SDL_BUTTON_LMASK) {
                        touchX = event.motion.x;
                        touchY = event.motion.y;
                    }
                }
            }
            else if (event.type == SDL_FINGERUP || event.type == SDL_MOUSEBUTTONUP) {
                touchMoving = 0;
            }
        }

        // DELTA TIME
        Uint32 currentTime = SDL_GetTicks();

        // MACHINE À ÉTATS
        if (g_gameState == MENU) {
            // RENDER
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);
            DrawMenu(renderer, g_font);
            SDL_RenderPresent(renderer);
        }
        else if (g_gameState == GAME) {
            // Touch-based ship movement
            if (touchMoving) {
                // Move ship towards touch position
                float dx = touchX - (g_ship.rect.x + g_ship.rect.w / 2);
                float dy = touchY - (g_ship.rect.y + g_ship.rect.h / 2);
                
                if (abs(dx) > SHIP_SPEED) {
                    if (dx > 0) {
                        g_ship.rect.x += SHIP_SPEED;
                        g_ship.vx = SHIP_SPEED;
                    } else {
                        g_ship.rect.x -= SHIP_SPEED;
                        g_ship.vx = -SHIP_SPEED;
                    }
                }
                
                if (abs(dy) > SHIP_SPEED) {
                    if (dy > 0) {
                        g_ship.rect.y += SHIP_SPEED;
                        g_ship.vy = SHIP_SPEED;
                    } else {
                        g_ship.rect.y -= SHIP_SPEED;
                        g_ship.vy = -SHIP_SPEED;
                    }
                }
                
                // Bounds checking
                if (g_ship.rect.x < 0) g_ship.rect.x = 0;
                if (g_ship.rect.x > WINDOW_WIDTH - g_ship.rect.w)
                    g_ship.rect.x = WINDOW_WIDTH - g_ship.rect.w;
                if (g_ship.rect.y < 0) g_ship.rect.y = 0;
                if (g_ship.rect.y > WINDOW_HEIGHT - g_ship.rect.h)
                    g_ship.rect.y = WINDOW_HEIGHT - g_ship.rect.h;
                
                // Create trail
                if (g_ship.vx != 0 || g_ship.vy != 0) {
                    CreateTrail(
                        g_ship.rect.x + g_ship.rect.w / 2,
                        g_ship.rect.y + g_ship.rect.h / 2,
                        0.0f, 1.0f,
                        (SDL_Color){135, 206, 235, 255}
                    );
                }
            } else {
                g_ship.vx = 0;
                g_ship.vy = 0;
            }

            // Ajuster le délai de spawn selon le score
            if (g_score > 50) spawnDelay = 1500;
            if (g_score > 100) spawnDelay = 1000;
            if (g_score > 200) spawnDelay = 700;

            if (currentTime - lastSpawn > spawnDelay) {
                SpawnEnemy();
                lastSpawn = currentTime;
            }

            // UPDATES
            UpdateStars();
            UpdateBullets();
            UpdateEnemies();
            UpdateTrails();
            UpdateShield();

            // CHECK COLLISIONS
            CheckBulletEnemyCollision();
            CheckShieldEnemyCollision();
            CheckMissileEnemyCollision();
            CheckShootEnemyCollision();
            CheckShipEnemyCollision();

            // RENDER
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);

            DrawStars(renderer);
            DrawBullets(renderer);
            DrawEnemies(renderer);
            DrawShip(renderer);
            DrawTrails(renderer);
            DrawTouchButtons(renderer);

            // HUD
            DrawLives(renderer, g_ship.lives, SHIP_MAX_LIVES);
            DrawScore(renderer, g_font, g_score);

            SDL_RenderPresent(renderer);

            if (g_ship.lives <= 0) {
                g_gameState = GAME_OVER;
            }
        }
        else if (g_gameState == GAME_OVER) {
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);
            DrawGameOver(renderer, g_font);
            SDL_RenderPresent(renderer);
        }

        SDL_Delay(FRAME_DELAY_MS);
    }

    // CLEAN
    if (g_font) TTF_CloseFont(g_font);
    if (g_shipTexture) SDL_DestroyTexture(g_shipTexture);
    for (int i = 0; i < 11; i++) {
        if (g_enemyTextures[i]) SDL_DestroyTexture(g_enemyTextures[i]);
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
    return 0;
}

void CheckShipEnemyCollision() {
    SDL_Rect shipRect = g_ship.rect;
    for (int i = 0; i < NUM_ENEMIES; i++) {
        if (g_enemies[i].active) {
            SDL_Rect enemyRect = {
                (int)g_enemies[i].x,
                (int)g_enemies[i].y,
                34, 34
            };
            if (SDL_HasIntersection(&shipRect, &enemyRect)) {
                if (!g_shieldActive) {
                    g_ship.lives--;
                }
                g_enemies[i].active = 0;
            }
        }
    }
}
