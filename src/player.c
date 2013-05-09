#include "svgestim.h"

#define	LOCAL_NAMESPACE
#include "player.h"


int player_spawn(int x, int y, SHAPE *shape) {
	int i;

	if (player)
		player_kill();

	if (!(player = malloc(sizeof(PLAYER)))) {
		fprintf(stderr, "Unable to alloc memory for player\n");
		return 0;
	}


	player->coord = memdup(shape->coordinate, (shape->lines  + 1) * 2 * sizeof(int));
	player->lines = shape->lines;
	player->line = d_render_line_new(shape->lines, 1);

	for (i = 0; i < shape->lines; i++)
		d_render_line_move(player->line, i, player->coord[i * 2], player->coord[i * 2 + 1], player->coord[i * 2 + 2], player->coord[i * 2 + 3]);
	
	player->x = x * 1000;
	player->y = y * 1000;
	
	return 1;
}


int player_loop() {
	DARNIT_KEYS keys;

	if (!player)
		return 0;
	
	keys = d_keys_get();
	if (keys.left)
		player->vel_x += PLAYER_ACCELERATION * d_last_frame_time();
	else if (keys.right)
		player->vel_x -= PLAYER_ACCELERATION * d_last_frame_time();
	else
		player->vel_x += (player->vel_x < 0 ? 1 : -1) * (PLAYER_ACCELERATION>>1) * d_last_frame_time();
	if (abs(player->vel_x) > PLAYER_SPEED_X_MAX)
		player->vel_x = (player->vel_x < 0 ? -1 : 1) * PLAYER_SPEED_X_MAX;
	
	player->x += player->vel_x * d_last_frame_time() * (keys.l ? 2 : 1);
	player->y += player->vel_y * d_last_frame_time();

	if (player->x / 1000 >= PLAYER_KILLZONE) {
		player_kill();
		return -1;
	}
		

	return 1;
}


void player_render() {
	d_render_offset(player->x / 1000, player->y / 1000);
	d_render_line_draw(player->line, ~0);

	return;
}


void player_kill() {
	free(player);
	player = NULL;

	return;
}
