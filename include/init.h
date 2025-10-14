#ifndef INIT_H
#define INIT_H

#if TARGET_OS_IPHONE
#include <SDL.h>
#else
#include <SDL2/SDL.h>
#endif
#include "globals.h" // Ajoutez cette ligne pour inclure la définition de Enemy

void InitStars();
void InitBullets();
void InitEnemies(SDL_Renderer* renderer);
void InitTrails();
void CheckBulletEnemyCollision();
void CheckShieldEnemyCollision();
Enemy* FindNearestEnemy(float x, float y);
void SpawnEnemy();

#endif // INIT_H
