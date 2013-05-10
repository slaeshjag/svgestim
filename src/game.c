#include "game.h"
#include "map.h"

void game_init() {
	shape_init("models.stz");
	model.player=shape_load("player");
	model.gun=shape_load("gun");
	model.bullet=shape_load("bullet");
}

void game_handle(DARNIT_KEYS *keys, DARNIT_MOUSE *mouse) {
	player_loop(keys);
}

void game_render() {
	map_render();
	player_render();
	particle_emitter_loop();
}
