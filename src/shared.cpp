#include "shared.h"

bool bullethell=false;

int RELOAD_TIME=500;

int SMALLER_DIMENSION=600;

int WIN_WIDTH=800;
int WIN_HEIGHT=600;

float cameraX=0.0, cameraY=0.0;

SDL_Window *gw;
SDL_Renderer *gr;

int score[2] = {0, 0};
int cacheX,cacheY;
int timer = 0;
void drawObject(float x, float y, float a, const float verticies[][2], size_t size, Uint8 r) {
	// prepare lines
	SDL_Point toDraw[size];
	for(int i=0; i < size; i++) {
		toDraw[i].x = toPixelX(cos(a)*verticies[i][0]-sin(a)*verticies[i][1]+x);
		toDraw[i].y = toPixelY(sin(a)*verticies[i][0]+cos(a)*verticies[i][1]+y);
	}

	// draw them with the specified color
	SDL_SetRenderDrawColor(gr, 255*(r&4), 255*(r&2), 255*(r&1), 255);
	SDL_RenderDrawLines(gr, toDraw, size);
}

void drawScore() {
	const float line[2][2] = {{0.0, 0.0}, {0.0, SCORE_SIZE}};
	for(int i = 0; i < score[1]; i++)
		drawObject(-SCALE/2+(i+1)*SCORE_SIZE, -SCALE/2, 0, line, 2, 7);
	for(int i = 0; i < score[0]; i++) 
		drawObject(SCALE/2-(i+1)*SCORE_SIZE, -SCALE/2, 0, line, 2, 7);
}

void drawBorder() {
	// this "on-first-glance" confusing fuckery is to save on cpu power
	float min=-SCALE/2-SHIP_SIZE;
	float max=SCALE/2+SHIP_SIZE;
	const float line[5][2] = {{min, min}, {min, max}, 
		{max, max}, {max, min}, {min, min}};
	drawObject(0,0,0,line,5,7);
}

void calcScreenRatio() {
	cacheX=0;
	cacheY=0;
	if(WIN_WIDTH>WIN_HEIGHT) {
		cacheX = (WIN_WIDTH-WIN_HEIGHT)/2;
		SMALLER_DIMENSION=WIN_HEIGHT;
	} else {
		cacheY = (WIN_HEIGHT-WIN_WIDTH)/2;
		SMALLER_DIMENSION=WIN_WIDTH;
	}
}

