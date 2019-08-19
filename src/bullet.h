#ifndef BULLET_H
#define BULLET_H

#include "player.h"

class bullet {
	protected:
		// x position, y position, angle, speed
		float x, y, a, s;
	private:
		// used to determine when to destroy bullets
		unsigned int creation;
		const float shape[2][2] = {{0.0, 0.0}, {0.0, SHIP_SIZE}};
	public:
		bool destroy = false;

		bullet(float px,float py,float pa, float ps, float shooterDiameter);
		void draw();
		void update(player *p1, player *p2);
};

#endif

