#ifndef PLAYER_H
#define PLAYER_H

class player {
	private:
		unsigned char color;
		unsigned int firingTime;
		const float shape[5][2] = {{0.0, -0.1}, {-0.1, 0.1}, {0.0, 0.0}, 
			{0.1, 0.1}, {0.0, -0.1}};
		// angle speed
		float as=0.0;
		bool ai;

	public:
		// directions follow forwards, backwards, left, right
		bool d[4];
		// x position, y position, angle, speed
		float x, y, a, s;
		bool alive;

		player(float sx, float sy, float sa, unsigned char scolor, bool isTerminator);
		void draw();
		void update();
		bool isOkForFiring();
		void noAI(void);
		bool isAISchoot(player *p);
};
#endif

