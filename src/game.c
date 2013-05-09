#include "game.h"
#include "map.h"

void game_init() {
	shape_init("models.stz");
	model.player=shape_load("player");
}

void game_handle(DARNIT_KEYS *keys, DARNIT_MOUSE *mouse) {
	player_loop(keys);
}

void game_render() {
	map_render();
	player_render();
}
