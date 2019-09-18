#ifndef SHARED_H
#define SHARED_H

#include <SDL2/SDL.h>

#define MIN(x,y) ( x>y? y : x )
#define MAX(x,y) ( x>y? x : y )

// macros = speed, because no stack allocations
#define toPixelX(x) (x+SCALE/2)*SMALLER_DIMENSION/SCALE+cacheX;
#define toPixelY(y) (y+SCALE/2)*SMALLER_DIMENSION/SCALE+cacheY;

#ifdef __PSP__
	#define OHPI 1.57
	#define THPI 4.71
	#define TPI 6.28 
	#define PI 3.14

	#define P1_DEF_A THPI
	#define P2_DEF_A OHPI

	// why the fuck is the sin in degrees and cos in radial
	// who the fuck thought out this shit
	#define sin(x) cos(x+PI/2)

	// PSP defaults to a integer one, so this just overrides the default behaviour
	#define abs(x) fabs(x)
#else
	#define PI M_PI
	#define OHPI PI*0.5
	#define THPI PI*1.5
	#define TPI PI*2 
	
	#define P1_DEF_A OHPI
	#define P2_DEF_A THPI
#endif


#define uint unsigned int

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

