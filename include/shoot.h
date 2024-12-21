#ifndef SHOOT_H
#define SHOOT_H

#include "globals.h" // Ajoutez cette ligne

void ShootBullet();
void ShootMissile(float offsetX);
Enemy* FindNearestEnemy(float x, float y);

#endif // SHOOT_H
