#include "shared.h"
#include "bullet.h"

bullet::bullet(float px,float py,float pa, float ps, float shooterDiameter) {
	// set only the "needed" defaults
	a=pa;
	s=0.15+ps;
	x=px+sin(a)*shooterDiameter;
	y=py-cos(a)*shooterDiameter;

	// use the creation time to determine when the bullet should be destroyed
	// bullet lifespan = 5 seconds
	creation = SDL_GetTicks();
}

void bullet::draw(){
	drawObject(x, y, a, shape, 2, 2);
}

void bullet::update(player *p1, player *p2) {
	// check if its too old to exist
	if(SDL_GetTicks()-creation>5000) destroy=true;

	// update position based on speed and angle
	x+=sin(a)*s;
	y-=cos(a)*s;

	// check for X-axis bounce
	if(abs(x)>SCALE/2) { 
		x=SCALE/2*(x>0.0?1:-1);
		s/=2;
		a=PI*2-a;
	}
	
	// check for Y-axis bounce
	if(abs(y)>SCALE/2) { 
		y=SCALE/2*(y>0.0?1:-1);
		s/=2;
		a=PI-a;
	}

	// player 1 hit!
	if(p1->alive&&sqrt(pow(p1->x-x,2)+pow(p1->y-y,2))<SHIP_SIZE) { 
		score[0]++;
		p1->alive = false;
		destroy = true;
		timer = SDL_GetTicks();
	}
	// player 2 hit!
	if(p2->alive&&sqrt(pow(p2->x-x,2)+pow(p2->y-y,2))<SHIP_SIZE) {
		score[1]++;
		p2->alive = false;
		destroy = true;
		timer = SDL_GetTicks();
	}
}

