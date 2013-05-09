#include "svgestim.h"
#include "menu.h"

static GAMESTATE _gamestate=GAMESTATE_MENU;
static void (*render[GAMESTATES])()={
	render_menu,
};

void gamestate(GAMESTATE state) {
	switch(_gamestate) {
		default:
			break;
	}
	_gamestate=state;
	switch(state) {
		default:
			break;
	}
}

static void init() {
	univox=d_font_load("univox.ttf", 16, 512, 512);
}

int main(int argc, char **argv) {
	DARNIT_KEYS keys;
	
	d_init_custom("svgestim", 800, 480, 0, "svgestim", NULL);
	init();
	
	gamestate(GAMESTATE_MENU);
	while(_gamestate!=GAMESTATE_QUIT) {
		keys=d_keys_get();
		d_render_begin();
		if(render[_gamestate])
			render[_gamestate]();
		d_render_end();
		d_loop();
	}
	
	d_quit();
	return 0;
}
