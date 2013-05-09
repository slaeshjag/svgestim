#include "svgestim.h"
#include "menu.h"
#include "map.h"
#include "game.h"

static GAMESTATE _gamestate=GAMESTATE_MENU;
static void (*render[GAMESTATES])()={
	[GAMESTATE_MENU]=menu_render,
	[GAMESTATE_GAME]=game_render,
};

static void (*handle[GAMESTATES])(DARNIT_KEYS *keys, DARNIT_MOUSE *mouse)={
	[GAMESTATE_MENU]=menu_handle,
	[GAMESTATE_GAME]=game_handle,
};

static DARNIT_INPUT_MAP menu_keymap_desktop={
	.a=TPW_KEY_F1,
	.b=TPW_KEY_F2,
	.x=TPW_KEY_F3,
	.y=TPW_KEY_F4,
};
static DARNIT_INPUT_MAP menu_keymap_handheld={
	.a=TPW_KEY_1,
	.b=TPW_KEY_2,
	.x=TPW_KEY_3,
	.y=TPW_KEY_4,
};

void gamestate(GAMESTATE state) {
	switch(_gamestate) {
		case GAMESTATE_MENU:
			d_keymapping_reset();
			break;
		default:
			break;
	}
	_gamestate=state;
	switch(state) {
		case GAMESTATE_MENU:
			if(platform.platform&DARNIT_PLATFORM_HANDHELD)
				d_keymapping_set(menu_keymap_handheld);
			else
				d_keymapping_set(menu_keymap_desktop);
			break;
		case GAMESTATE_GAME:
			map_load(0);
			break;
		default:
			break;
	}
}

static void init() {
	d_init_custom("svgestim", 800, 480, 0, "svgestim", NULL);
	platform=d_platform_get();
	
	font.univox=d_font_load("univox.ttf", 32, 512, 512);
	font.vectroid=d_font_load("vectroid.ttf", 32, 512, 512);
	
	menu_init();
	//game_init();
	//pause_init();
	map_init();
	d_render_blend_enable();
}

int main(int argc, char **argv) {
	DARNIT_KEYS keys;
	DARNIT_MOUSE mouse;
	particle_emitter_new(30, 5000, 8000, 10000, 255, 0, 0, PARTICLE_TYPE_PULSE, 400, 240, 0, -100, 100);
	particle_emitter_new(30, 5000, 8000, 10000, 255, 255, 0, PARTICLE_TYPE_PULSE, 400, 240, 0, -100, 100);
	
	init();
	gamestate(GAMESTATE_MENU);
	
	while(_gamestate!=GAMESTATE_QUIT) {
		keys=d_keys_get();
		mouse=d_mouse_get();
		
		if(handle[_gamestate])
			handle[_gamestate](&keys, &mouse);
		d_render_begin();
		if(render[_gamestate])
			render[_gamestate]();
		particle_emitter_loop();
		d_render_end();
		d_loop();
	}
	
	d_quit();
	return 0;
}
