#ifndef DRAW_H
#define DRAW_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

void DrawStars(SDL_Renderer* renderer);
void DrawBullets(SDL_Renderer* renderer);
void DrawEnemies(SDL_Renderer* renderer);
void DrawShip(SDL_Renderer* renderer);
void DrawLives(SDL_Renderer* renderer, int lives, int maxLives);
void DrawScore(SDL_Renderer* renderer, TTF_Font* font, int score);
void DrawMenu(SDL_Renderer* renderer, TTF_Font* font);
void DrawGameOver(SDL_Renderer* renderer, TTF_Font* font);
void DrawTrails(SDL_Renderer* renderer);
void DrawFilledCircle(SDL_Renderer* renderer, int x, int y, int radius, SDL_Color color);

#endif // DRAW_H
