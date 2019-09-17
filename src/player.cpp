#include "shared.h"
#include "player.h"

player::player(float sx, float sy, float sa, unsigned char scolor, bool isTerminator) {
	// bullethell mode works better with a different firing rate
	firingTime=SDL_GetTicks()+(bullethell?2000:5000);

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
	a=fmod(a+PI, TPI)-PI;
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

	// dereferencing object may improve speed (by making less memory reads)
	const float ex=p->x, ey=p->y, ea=p->a, es=p->s;

	float bulletX, bulletY;
	float enemyX, enemyY;
	float count=1.0;
	for(uint i=0; i<30; i++) {
		bulletX = x+count*sin(a)*0.15;
		bulletY = y-count*cos(a)*0.15;

		float futureSpeed = es;
		if(p->d[0]) {
			futureSpeed+=(count-1.0)*0.0005;
			if(futureSpeed>0.05) futureSpeed=0.05;
		} else {
			futureSpeed*=pow(0.99, count);
		}
		enemyX = ex+count*sin(ea)*(es+futureSpeed)*0.5;
		enemyY = ey-count*cos(ea)*(es+futureSpeed)*0.5;

		if(sqrt(powf(bulletX-enemyX, 2)+powf(bulletY-enemyY, 2)) < SHIP_SIZE) return 1;
		count+=1.0;
	}

	float b = x - p->x;
	float c = y - p->y;
	float angleAlpha = atan(c/b) + acos(b*c/(fabs(b)*fabs(c)));
	if(c<0) angleAlpha -= PI;
#ifdef __PSP__
	angleAlpha+=fmod(a-PI, TPI)-OHPI;
#else
	angleAlpha-=fmod(a-PI, TPI)+THPI;
#endif
	angleAlpha=fmod(angleAlpha-PI, TPI)+PI;

	// printf debugging, don't ask
	//printf("angleAlpha: %f %f\n", a, angleAlpha);

	// floating point hell
	if(!isnan(angleAlpha)) {
		if(angleAlpha>as) {
#ifdef __PSP__
			d[3] = rand()%10>8 ? false : true;
#else
			d[3] = true;
#endif
			d[2] = false;
		} else if(angleAlpha<-as) {
			d[3] = false;
#ifdef __PSP__
			d[2] = rand()%10>8 ? false : true;
#else
			d[2] = true;
#endif
		} else {
			// knowing floating point, this code would never run
			d[2] = false;
			d[3] = false;
		}
	}
#ifndef __PSP__
	as+=0.0000001*(rand()&0xFF);
#endif
	d[0] = true;
	d[1] = false;

	return 0;
}

void player::noAI(void) {
	if(!ai) return;
	firingTime-=3000;
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

