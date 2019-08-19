#ifndef SHARED_H
#define SHARED_H

#include <SDL2/SDL.h>

#define MIN(x,y) ( x>y? y : x )
#define MAX(x,y) ( x>y? x : y )

// macros = speed, because no stack allocations
#define toPixelX(x) (x+SCALE/2)*SMALLER_DIMENSION/SCALE+cacheX;
#define toPixelY(y) (y+SCALE/2)*SMALLER_DIMENSION/SCALE+cacheY;

#define PI M_PI

#define SCALE 10
#define SHIP_SIZE 0.1
#define SCORE_SIZE 0.4

extern bool bullethell;

extern int RELOAD_TIME;

extern int WIN_WIDTH;
extern int WIN_HEIGHT;
extern int SMALLER_DIMENSION;

extern float cameraX, cameraY;
extern SDL_Window *gw;
extern SDL_Renderer *gr;

extern int score[2];
extern int cacheX,cacheY;
extern int timer;

void drawObject(float x, float y, float a, const float verticies[][2], size_t size, Uint8 r);
void drawScore();
void drawBorder();
void calcScreenRatio();

#endif

