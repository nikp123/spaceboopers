#include "shared.h"
#include "player.h"

player::player(float sx, float sy, float sa, unsigned char scolor, bool isTerminator) {
	// bullethell mode works better with a different firing rate
	firingTime=SDL_GetTicks()+(bullethell?2000:500);

	// reset all the "needed" values to defaults
	x=sx;
	y=sy;
	a=sa;
	s=0.0;
	alive=true;
	color=scolor;
	ai=isTerminator;
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
	if(x>SCALE/2||x<-SCALE/2) { 
		x=SCALE/2*(x>0.0?1:-1);
		s/=-2;
		a+=PI;
	}

	// handle bounce on Y axis
	y-=cos(a)*s;
	if(y>SCALE/2||y<-SCALE/2) { 
		y=SCALE/2*(y>0.0?1:-1);
		s/=-2;
		a+=PI;
	}

	// calculate angle changes
	a-=PI/6*as;

	// avoid possible float errors by wrapping around the angle
	a=fmod(a+PI, PI*2)-PI;
}

bool player::isAISchoot(player *p) {
	if(!ai) return 0;
	if(!p->alive) {
		d[0] = false;
		d[1] = false;
		d[2] = false;
		d[3] = false;
		return 0;
	}

	float bulletX, bulletY;
	float enemyX, enemyY;
	for(int i=0; i<60; i++) {
		bulletX = x+sin(a)*0.15*(i+1);
		bulletY = y-cos(a)*0.15*(i+1);

		float futureSpeed = p->s;
		if(p->d[0]) {
			futureSpeed+=i*0.0005;
			if(futureSpeed>0.05) futureSpeed=0.05;
		} else {
			futureSpeed*=pow(0.99, i+1);
		}
		enemyX = p->x+sin(p->a)*(p->s+futureSpeed)/2*(i+1);
		enemyY = p->y-cos(p->a)*(p->s+futureSpeed)/2*(i+1);

		if(sqrt(powf(bulletX-enemyX, 2)+powf(bulletY-enemyY, 2)) < SHIP_SIZE) return 1;
	}

	float b = x - p->x;
	float c = y - p->y;
	float angleAlpha = atan(c/b) + acos(b*c/(fabs(b)*fabs(c)));
	if(c<0) angleAlpha -= PI;
#ifdef __PSP__
	angleAlpha+=fmod(a-PI, PI*2.0)-PI/2.0;
#else
	angleAlpha-=fmod(a-PI, PI*2.0)+PI*1.5;
#endif
	angleAlpha=fmod(angleAlpha-PI, PI*2.0)+PI;

	// printf debugging, don't ask
	//printf("angleAlpha: %f %f\n", a, angleAlpha);

	// floating point hell
	if(!isnan(angleAlpha)) {
		if(angleAlpha>as) {
			d[3] = true;
			d[2] = false;
		} else if(angleAlpha<-as) {
			d[3] = false;
			d[2] = true;
		} else {
			// knowing floating point, this code would never run
			d[2] = false;
			d[3] = false;
		}
	}
	d[0] = true;
	d[1] = false;

	return 0;
}

void player::noAI(void) {
	if(!ai) return;
	ai=false;
	d[0] = false;
	d[1] = false;
	d[2] = false;
	d[3] = false;
}

bool player::isOkForFiring() {
	if(SDL_GetTicks()>RELOAD_TIME+firingTime) {
		firingTime=SDL_GetTicks();

		// we don't want the player to be able to fire whilst being dead
		return alive;
	}
	return false;
}

