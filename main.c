#include "include/init.h"
#include "include/update.h"
#include "include/draw.h"
#include "include/shoot.h"
#include "include/globals.h"
#include <SDL2/SDL_image.h> // Ajoutez cette ligne
#include <stdio.h> // Ajoutez cette ligne pour les impressions de débogage

//-------------------------------
// CONFIG & CONSTANTES
//-------------------------------
#define WINDOW_WIDTH          375
#define WINDOW_HEIGHT         667
#define NUM_STARS             50   // Nombre d'étoiles
#define NUM_BULLETS           50
#define NUM_ENEMIES           20
#define FRAME_DELAY_MS        16   // ~60 FPS
#define NUM_TRAILS            100

// Vitesse de déplacement du vaisseau
#define SHIP_SPEED            5
// Nombre de vies max
#define SHIP_MAX_LIVES        3

//-------------------------------
// FONCTION PRINCIPALE
//-------------------------------
void CheckBulletEnemyCollision();
void CheckShieldEnemyCollision();
void CheckMissileEnemyCollision(); // Ajoutez cette ligne
void CheckShootEnemyCollision();   // Ajoutez cette ligne
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
int g_shieldActive = 0; // Bouclier
SDL_Texture* g_shipTexture   = NULL;
SDL_Texture* g_enemyTextures[11] = {NULL};
TTF_Font*    g_font          = NULL;

// Modification de la fonction CreateTrail pour empêcher les particules d'être au-dessus du vaisseau
void CreateTrail(float x, float y, float vx, float vy, SDL_Color color) {
    // S'assurer que y n'est pas au-dessus du vaisseau
    if (y < g_ship.rect.y + g_ship.rect.h) { // Changement de + h/2 à + h
        y = g_ship.rect.y + g_ship.rect.h;
    }
    
    for (int i = 0; i < NUM_TRAILS; i++) {
        if (!g_trails[i].active) {
            g_trails[i].x = x;
            g_trails[i].y = y;
            g_trails[i].vx = vx;
            g_trails[i].vy = vy;
            g_trails[i].color = color;
            g_trails[i].lifetime = 60; // par exemple, 60 frames (~1 seconde à 60 FPS)
            g_trails[i].active = 1;
            break;
        }
    }
}

int main() {
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

    // Créer la fenêtre
    SDL_Window* window = SDL_CreateWindow("Space Invaders",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH, WINDOW_HEIGHT,
        SDL_WINDOW_SHOWN);
    if (!window) {
        printf("Erreur: SDL_CreateWindow: %s\n", SDL_GetError());
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
        return EXIT_FAILURE;
    }

    // Créer le renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
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

    // Charger une texture de vaisseau (optionnel)
    g_shipTexture = IMG_LoadTexture(renderer, "assets/2X/tiny_ship17.png");
    if (!g_shipTexture) {
        printf("Avertissement: impossible de charger la texture du vaisseau : %s\n", IMG_GetError());
        // Ce n'est pas bloquant, on peut dessiner un rectangle à la place.
    }

    // Charger la police
    g_font = TTF_OpenFont("assets/arial.ttf", 16);
    if (!g_font) {
        printf("Avertissement: TTF_OpenFont: %s\n", TTF_GetError());
        // On peut ignorer, mais le texte ne s'affichera pas si la police est introuvable.
    }

    // Init tout
    InitStars();
    InitBullets();
    InitEnemies(renderer); // Passer le renderer pour charger les textures
    InitTrails();

    // Variables de boucle
    int running = 1;
    Uint32 lastSpawn = 0; // pour spawn ennemi

    while (running) {
        // EVENTS
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            }
            else if (event.type == SDL_KEYDOWN) {
                // ENTER : passer MENU -> GAME ou GAME_OVER -> GAME
                if (g_gameState == MENU || g_gameState == GAME_OVER) {
                    if (event.key.keysym.sym == SDLK_RETURN) {
                        g_gameState = GAME;
                        g_ship.lives = SHIP_MAX_LIVES;
                        g_score      = 0;
                        // On remet le vaisseau au milieu
                        g_ship.rect.x = WINDOW_WIDTH / 2 - g_ship.rect.w/2;
                        g_ship.rect.y = WINDOW_HEIGHT - 50;
                    }
                }
                if (event.key.keysym.sym == SDLK_e) {
                    g_shieldActive = !g_shieldActive;
                }
                if (event.key.keysym.sym == SDLK_a) {
                    ShootMissile(-10); // Missile gauche
                    ShootMissile(10);  // Missile droit
                    // Créer une trainée violette lors du tir de missile
                    CreateTrail(
                        g_ship.rect.x + g_ship.rect.w / 2,
                        g_ship.rect.y + g_ship.rect.h / 2,
                        0.0f,  // Vélocité horizontale de la trainée
                        1.0f,  // Vélocité verticale de la trainée (vers le bas)
                        (SDL_Color){128, 0, 128, 255} // Couleur violette
                    );
                }
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
            // Déplacements du vaisseau
            const Uint8* state = SDL_GetKeyboardState(NULL);
            if (state[SDL_SCANCODE_LEFT]) {
                g_ship.rect.x -= SHIP_SPEED;
                g_ship.vx = -SHIP_SPEED;
                if (g_ship.rect.x < 0) g_ship.rect.x = 0;
                // Créer une trainée bleu ciel lors du déplacement vers la gauche
                CreateTrail(
                    g_ship.rect.x + g_ship.rect.w / 2,
                    g_ship.rect.y + g_ship.rect.h / 2,
                    0.0f,  // Vélocité horizontale de la trainée
                    1.0f,  // Vélocité verticale de la trainée (vers le bas)
                    (SDL_Color){135, 206, 235, 255} // Couleur bleu ciel
                );
            } else if (state[SDL_SCANCODE_RIGHT]) {
                g_ship.rect.x += SHIP_SPEED;
                g_ship.vx = SHIP_SPEED;
                if (g_ship.rect.x > WINDOW_WIDTH - g_ship.rect.w) 
                    g_ship.rect.x = WINDOW_WIDTH - g_ship.rect.w;
                // Créer une trainée bleu ciel lors du déplacement vers la droite
                CreateTrail(
                    g_ship.rect.x + g_ship.rect.w / 2,
                    g_ship.rect.y + g_ship.rect.h / 2,
                    0.0f,  // Vélocité horizontale de la trainée
                    1.0f,  // Vélocité verticale de la trainée (vers le bas)
                    (SDL_Color){135, 206, 235, 255} // Couleur bleu ciel
                );
            }
            
            if (state[SDL_SCANCODE_UP]) {
                g_ship.rect.y -= SHIP_SPEED;
                g_ship.vy = -SHIP_SPEED;
                if (g_ship.rect.y < 0) g_ship.rect.y = 0;
                // Créer une trainée bleu ciel lors du déplacement vers le haut
                CreateTrail(
                    g_ship.rect.x + g_ship.rect.w / 2,
                    g_ship.rect.y + g_ship.rect.h / 2,
                    0.0f,  // Vélocité horizontale de la trainée
                    1.0f,  // Vélocité verticale de la trainée (vers le bas)
                    (SDL_Color){135, 206, 235, 255} // Couleur bleu ciel
                );
            } else if (state[SDL_SCANCODE_DOWN]) {
                g_ship.rect.y += SHIP_SPEED;
                g_ship.vy = SHIP_SPEED;
                if (g_ship.rect.y > WINDOW_HEIGHT - g_ship.rect.h)
                    g_ship.rect.y = WINDOW_HEIGHT - g_ship.rect.h;
                // Créer une trainée bleu ciel lors du déplacement vers le bas
                CreateTrail(
                    g_ship.rect.x + g_ship.rect.w / 2,
                    g_ship.rect.y + g_ship.rect.h / 2,
                    0.0f,  // Vélocité horizontale de la trainée
                    1.0f,  // Vélocité verticale de la trainée (vers le bas)
                    (SDL_Color){135, 206, 235, 255} // Couleur bleu ciel
                );
            } else {
                g_ship.vx = 0;
                g_ship.vy = 0;
            }

            // Tirs
            if (state[SDL_SCANCODE_SPACE]) {
                ShootBullet();
            }
            
            // Spawning d'ennemis toutes les 2s
            if (currentTime - lastSpawn > 2000) {
                SpawnEnemy();
                lastSpawn = currentTime;
            }

            // UPDATES
            UpdateStars();
            UpdateBullets();
            UpdateEnemies();
            UpdateTrails(); // Assurez-vous que UpdateTrails est appelé
            UpdateShield(); // Ajoutez cette ligne pour mettre à jour le bouclier

            // CHECK COLLISIONS
            CheckBulletEnemyCollision();
            CheckShieldEnemyCollision(); // Ajout de la vérification des collisions du bouclier
            CheckMissileEnemyCollision(); // Ajout de la vérification des collisions des missiles
            CheckShootEnemyCollision();   // Ajout de la vérification des collisions des tirs

            // RENDER
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);

            // Dessin
            DrawStars(renderer);      // Fond d'étoiles
            DrawBullets(renderer);    // Bullets
            DrawEnemies(renderer);    // Ennemis
            DrawShip(renderer);       // Vaisseau
            DrawTrails(renderer);     // Dessin des traînées

            // HUD : vies + score
            DrawLives(renderer, g_ship.lives, SHIP_MAX_LIVES);
            DrawScore(renderer, g_font, g_score);

            SDL_RenderPresent(renderer);

            // Si plus de vies, GAME_OVER
            if (g_ship.lives <= 0) {
                g_gameState = GAME_OVER;
            }
        }
        else if (g_gameState == GAME_OVER) {
            // RENDER
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);
            DrawGameOver(renderer, g_font);
            SDL_RenderPresent(renderer);
        }

        SDL_Delay(FRAME_DELAY_MS);
    }

    // CLEAN
    if (g_font)          TTF_CloseFont(g_font);
    if (g_shipTexture)   SDL_DestroyTexture(g_shipTexture);
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