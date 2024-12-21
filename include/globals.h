#ifndef GLOBALS_H
#define GLOBALS_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#define WINDOW_WIDTH          375
#define WINDOW_HEIGHT         667
#define NUM_STARS             50
#define NUM_BULLETS           50
#define NUM_ENEMIES           20
#define FRAME_DELAY_MS        16
#define NUM_TRAILS            100
#define SHIP_SPEED            5
#define SHIP_MAX_LIVES        3

typedef enum {
    MENU,
    GAME,
    GAME_OVER
} GameState;

typedef struct {
    int   x, y;
    float speed;
} Star;

typedef struct {
    float x, y;
    float vx, vy;
    SDL_Rect hitbox;
    int active;
} Enemy;

typedef struct {
    SDL_Rect rect;
    int       lives;
    float vx;
    float vy;
} Ship;

typedef enum {
    BULLET,
    MISSILE
} BulletType;

typedef struct {
    float x, y;
    float vy;
    float vx;
    float targetX;
    float targetY;
    int   active;
    SDL_Rect hitbox;
    BulletType type; // Ajoutez cette ligne
} Bullet;

typedef struct {
    float x, y;
    float vx, vy;
    int active;
    SDL_Color color;
    int lifetime;
} TrailParticle;

extern GameState  g_gameState;
extern Ship       g_ship;
extern Star       g_stars[NUM_STARS];
extern Bullet     g_bullets[NUM_BULLETS];
extern Enemy      g_enemies[NUM_ENEMIES];
extern TrailParticle g_trails[NUM_TRAILS];
extern int  g_score;
extern int g_shieldActive;
extern SDL_Texture* g_shipTexture;
extern SDL_Texture* g_enemyTextures[11];
extern TTF_Font*    g_font;
extern float g_shieldPulse; // Ajoutez cette ligne

void CheckShieldEnemyCollision();
void SpawnEnemy();
Enemy* FindNearestEnemy(float x, float y);
void CreateTrail(float x, float y, float vx, float vy, SDL_Color color);
void UpdateShield(); // Ajoutez cette ligne

#endif // GLOBALS_H
