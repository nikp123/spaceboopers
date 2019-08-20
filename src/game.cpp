#include <iostream>
#include <vector>
#include <memory>

#include "shared.h"
#include "bullet.h"
#include "player.h"

static bool ripmygame = false;

static player *p[2];
static std::vector<bullet*> b;

static SDL_Joystick **joystick;

void init() {
	SDL_SetHint(SDL_HINT_ACCELEROMETER_AS_JOYSTICK, "0");

	SDL_Init(SDL_INIT_VIDEO|SDL_INIT_JOYSTICK);
	gw=SDL_CreateWindow("Spaceboopers!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
			WIN_WIDTH, WIN_HEIGHT, SDL_WINDOW_RESIZABLE);
	gr=SDL_CreateRenderer(gw,-1,0);

	joystick = new SDL_Joystick*[SDL_NumJoysticks()];
	for(int i=0; i<SDL_NumJoysticks(); i++) {
		joystick[i] = SDL_JoystickOpen(i);
	}

	SDL_GetWindowSize(gw, &WIN_WIDTH, &WIN_HEIGHT);
	calcScreenRatio();
}

void start() {
	p[0] = new player(-SCALE/2+SHIP_SIZE, 0.0, PI/2, 4);
	p[1] = new player(SCALE/2-SHIP_SIZE, 0.0, PI*1.5, 1);

	// avoids a dumb stuck button bug
	for(int i=0; i<2; i++)
		for(int j=0; j<4; j++)
			p[i]->d[j] = false;

	if(bullethell) RELOAD_TIME = 200;
}

void restart() {
	while(b.size()) {
		delete b[0];
		b.erase(b.begin());
	}
	p[0]->~player(); 
	p[1]->~player(); 
	new(p[0]) player(-SCALE/2+SHIP_SIZE, 0.0, PI/2, 4);
	new(p[1]) player(SCALE/2-SHIP_SIZE, 0.0, PI*1.5, 1);
}

void events() {
	SDL_Event e;
	while(SDL_PollEvent(&e)){
		switch(e.type){
			case SDL_QUIT:
				ripmygame = true;
				break;
			case SDL_JOYAXISMOTION:
			{
				bool positive = e.jaxis.value>0? true : false;
				if(e.jaxis.value!=0) {
					if(e.jaxis.axis==1) {
						if(positive) {
							p[e.jaxis.which]->d[1] = true;
							p[e.jaxis.which]->d[0] = false;
						} else {
							p[e.jaxis.which]->d[0] = true;
							p[e.jaxis.which]->d[1] = false;
						}
					} else {
						if(positive) {
							p[e.jaxis.which]->d[3] = true;
							p[e.jaxis.which]->d[2] = false;
						} else {
							p[e.jaxis.which]->d[2] = true;
							p[e.jaxis.which]->d[3] = false;
						}
					}
				} else {
					if(e.jaxis.axis==1) {
						p[e.jaxis.which]->d[0] = false;
						p[e.jaxis.which]->d[1] = false;
					} else if(e.jaxis.axis==0) {
						p[e.jaxis.which]->d[2] = false;
						p[e.jaxis.which]->d[3] = false;
					}
				}
				break;
			}
			case SDL_JOYBUTTONDOWN:
			{
				int n = e.jbutton.which;
				if(p[n]->alive&&p[n]->isOkForFiring()) {
					b.push_back(new bullet
						(p[n]->x, p[n]->y,
						 p[n]->a, p[n]->s, 0.2));
				}
				break;
			}
			case SDL_KEYDOWN:
				switch(e.key.keysym.scancode) {
					case SDL_SCANCODE_W:     p[0]->d[0] = true; break;
					case SDL_SCANCODE_S:     p[0]->d[1] = true; break;
					case SDL_SCANCODE_A:     p[0]->d[2] = true; break;
					case SDL_SCANCODE_D:     p[0]->d[3] = true; break;
					case SDL_SCANCODE_UP:    p[1]->d[0] = true; break;
					case SDL_SCANCODE_DOWN:  p[1]->d[1] = true; break;
					case SDL_SCANCODE_LEFT:  p[1]->d[2] = true; break;
					case SDL_SCANCODE_RIGHT: p[1]->d[3] = true; break;
					case SDL_SCANCODE_LCTRL:
						if(p[0]->isOkForFiring()) {
							b.push_back(new bullet
								(p[0]->x, p[0]->y,
								 p[0]->a, p[0]->s, 0.2));
						}
						break;
					case SDL_SCANCODE_RCTRL:
						if(p[1]->isOkForFiring()) {
							b.push_back(new bullet
								(p[1]->x, p[1]->y,
								 p[1]->a, p[1]->s, 0.2));
						}
						break;
				}
				break;
			case SDL_KEYUP:
				switch(e.key.keysym.scancode) {
					case SDL_SCANCODE_W:     p[0]->d[0] = false; break;
					case SDL_SCANCODE_S:     p[0]->d[1] = false; break;
					case SDL_SCANCODE_A:     p[0]->d[2] = false; break;
					case SDL_SCANCODE_D:     p[0]->d[3] = false; break;
					case SDL_SCANCODE_UP:    p[1]->d[0] = false; break;
					case SDL_SCANCODE_DOWN:  p[1]->d[1] = false; break;
					case SDL_SCANCODE_LEFT:  p[1]->d[2] = false; break;
					case SDL_SCANCODE_RIGHT: p[1]->d[3] = false; break;
				}
				break;
			case SDL_WINDOWEVENT:
				if(e.window.event == SDL_WINDOWEVENT_CLOSE) ripmygame=true; 
				else if(e.window.event == SDL_WINDOWEVENT_RESIZED) {
					// if the user resized the window
					WIN_WIDTH = e.window.data1;
					WIN_HEIGHT = e.window.data2;
					calcScreenRatio();
				}
				break;
		}
	}
}

void update() {
	if(timer&&(SDL_GetTicks()-timer>2000)) {
		timer=0;
		std::cout << "Score " << score[1] << "-" << score[0] << std::endl;
		if(score[0]>9||score[1]>9) {
			std::cout << "Player " << (int)(score[0]>score[1]? 2:1) 
				<< " wins!" << std::endl;
			score[0]=0;
			score[1]=0;
		}
		restart();
	}
	if(p[0]->alive) p[0]->update();
	if(p[1]->alive) p[1]->update();
	
	if(bullethell) {
		if(p[0]->isOkForFiring()) {
			b.push_back(new bullet
				(p[0]->x, p[0]->y, p[0]->a, p[0]->s, 0.2));
		}
		if(p[1]->isOkForFiring()) {
			b.push_back(new bullet
				(p[1]->x, p[1]->y, p[1]->a, p[1]->s, 0.2));
		}
	}
	
	for(int i=0; i<b.size();) {
		b[i]->update(p[0], p[1]);
		if(b[i]->destroy) {
			delete b[i];
			b.erase(b.begin()+i);
		}
		else i++;
	}
}

void draw() {
	SDL_SetRenderDrawColor(gr, 0, 0, 0, 0);
	SDL_RenderClear(gr);
	for(int i=0; i<b.size(); i++) b[i]->draw();
	if(p[0]->alive) p[0]->draw();
	if(p[1]->alive) p[1]->draw();
	drawScore();
	drawBorder();
	SDL_RenderPresent(gr);
}

void cleanup() {
	for(int i=0; i<SDL_NumJoysticks(); i++)
		SDL_JoystickClose(joystick[i]);
	delete joystick;

	while(b.size()) {
		delete b[0];
		b.erase(b.begin());
	}
	p[0]->~player(); 
	p[1]->~player();

	SDL_DestroyRenderer(gr);
	SDL_DestroyWindow(gw);

	SDL_Quit();
}

int main(int argc, char *argv[]) {
	if(argc>1) {
		if(!strcmp(argv[1], "bullethell")) {
			bullethell = true;
		}
	}
	init();
	start();

	while(!ripmygame) {
		Uint32 beginFrame = SDL_GetTicks();
		events();
		update();
		draw();
		Uint32 delta = SDL_GetTicks()-beginFrame;
		SDL_Delay(delta>1000/60? 0 : 1000/60-delta);
	}

	cleanup();
	return 0;
}

