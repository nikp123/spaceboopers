#include "shared.h"
#include "player.h"

player::player(float sx, float sy, float sa, unsigned char scolor) {
	// bullethell mode works better with a different firing rate
	firingTime=SDL_GetTicks()+(bullethell?2000:500);

	// reset all the "needed" values to defaults
	x=sx;
	y=sy;
	a=sa;
	s=0.0;
	alive=true;
	color=scolor;
}

void player::draw() {
	drawObject(x, y, a, shape, 5, color);
}

void player::update() {
	// handle acceleration
	if(!(d[0]&&d[1])) {
		if(d[0]) if(s<0.05) s+=0.0005;
		if(d[1]) if(s>-0.05) s-=0.0005;
	}

	// slowly decrease speeds when idle
	if(!(d[0]||d[1])) {
		if(s!=0.0) s*=0.99;
	}

	// handle angle/turning acceleration
	if(!(d[2]&&d[3])) {
		if(d[2]) if(as<1.0) as+=0.01;
		if(d[3]) if(as>-1.0) as-=0.01;
	}

	// limit turning at high speeds
	as-=as*(abs(s)/0.5);

	// slowly decrease angle speed when idle
	if(!(d[2]||d[3])) {
		if(as!=0.0) as*=0.9;
	}
	
	// handle bounce on X axis
	x+=sin(a)*s;
	if(abs(x)>SCALE/2) { 
		x=SCALE/2*(x>0.0?1:-1);
		s/=-2;
	}

	// handle bounce on Y axis
	y-=cos(a)*s;
	if(abs(y)>SCALE/2) { 
		y=SCALE/2*(y>0.0?1:-1);
		s/=-2;
	}

	// calculate angle changes
	a-=PI/6*as;

	// avoid possible float errors by wrapping around the angle
	a=fmod(a, PI*2);
}

bool player::isOkForFiring() {
	if(SDL_GetTicks()>RELOAD_TIME+firingTime) {
		firingTime=SDL_GetTicks();

		// we don't want the player to be able to fire whilst being dead
		return alive;
	}
	return false;
}

