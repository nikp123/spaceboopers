#include "shared.h"

int toPixelX(float x) {
	return (x+SCALE/2)*WIN_HEIGHT/SCALE+cacheX;
}

int toPixelY(float y) {
	return (y+SCALE/2)*WIN_HEIGHT/SCALE+cacheY;
}

void drawObject(float x, float y, float a, const float verticies[][2], size_t size, Uint8 r) {
	SDL_Point toDraw[size];
	for(int i=0; i < size; i++) {
		toDraw[i].x = toPixelX(cos(a)*verticies[i][0]-sin(a)*verticies[i][1]+x);
		toDraw[i].y = toPixelY(sin(a)*verticies[i][0]+cos(a)*verticies[i][1]+y);
	}

	SDL_SetRenderDrawColor(gr, 255*(r&4), 255*(r&2), 255*(r&1), 255);

	SDL_RenderDrawLines(gr, toDraw, size);
}

void drawScore(){
	const float line[2][2] = {{0.0, 0.0}, {0.0, SCORE_SIZE}};
	for(int i = 0; i < score[1]; i++) drawObject(-SCALE/2+(i+1)*SCORE_SIZE, -SCALE/2, 0, line, 2, 7);
	for(int i = 0; i < score[0]; i++) drawObject(SCALE/2-(i+1)*SCORE_SIZE, -SCALE/2, 0, line, 2, 7);
}

void drawBorder() {
	float min=-SCALE/2-SHIP_SIZE;
	float max=SCALE/2+SHIP_SIZE;
	const float line[5][2] = {{min, min}, {min, max}, {max, max}, {max, min}, {min, min}};
	drawObject(0,0,0,line,5,7);
}

void calcScreenRatio() {
	cacheX=0;
	cacheY=0;
	if(WIN_WIDTH>WIN_HEIGHT) 
		cacheX = (WIN_WIDTH-WIN_HEIGHT)/2;
	else cacheY = (WIN_HEIGHT-WIN_WIDTH)/2;
}

