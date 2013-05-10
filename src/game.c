#include "game.h"
#include "map.h"

void game_init() {
	shape_init("models.stz");
	model.player=shapesprite_new("player walk", 2);
	model.gun=shape_load("gun");
	model.bullet=shape_load("bullet");
	model.enemy=shape_load("player");
}

void game_handle(DARNIT_KEYS *keys, DARNIT_MOUSE *mouse) {
	map_loop();
	player_loop(keys);
}

void game_render() {
	map_render();
	player_render();
	particle_emitter_loop();
}
