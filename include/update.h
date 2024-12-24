#ifndef UPDATE_H
#define UPDATE_H

void UpdateStars();
void UpdateBullets();
void UpdateEnemies();
void UpdateTrails();
void CheckBulletEnemyCollision();
void CheckShieldEnemyCollision();
void SpawnEnemy();
void CheckShipEnemyCollision();
void CreateExplosion(float x, float y);

#endif // UPDATE_H
