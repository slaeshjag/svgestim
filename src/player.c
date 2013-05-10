#include "svgestim.h"

#define	LOCAL_NAMESPACE
#include "player.h"


int player_spawn(int x, int y, SHAPE *shape) {
	if (player)
		player_kill();

	if (!(player = malloc(sizeof(PLAYER)))) {
		fprintf(stderr, "Unable to alloc memory for player\n");
		return 0;
	}

	player->shape = shape_copy_copy(shape);

	player->x = x * 1000;
	player->y = y * 1000;
	player->vel_x = 0;
	player->vel_y = 0;
	
	return 1;
}


int player_loop(DARNIT_KEYS *keys) {
	if (!player)
		return 0;
	
	if (keys->left)
		player->vel_x -= PLAYER_ACCELERATION * d_last_frame_time();
	else if (keys->right)
		player->vel_x += PLAYER_ACCELERATION * d_last_frame_time();
	else if (player->vel_x)
		player->vel_x += (abs(player->vel_x) > (PLAYER_ACCELERATION) * d_last_frame_time()) ? ((player->vel_x < 0 ? 1 : -1) * (PLAYER_ACCELERATION) * d_last_frame_time()) : -player->vel_x;
	if (abs(player->vel_x) > PLAYER_SPEED_X_MAX)
		player->vel_x = ((player->vel_x < 0 ? -1 : 1) * PLAYER_SPEED_X_MAX);
	
	player->x += (player->vel_x * d_last_frame_time() * (keys->l ? 2 : 1)) / 1000;
	player->y += (player->vel_y * d_last_frame_time()) / 1000;

	if (player->x / 1000 >= PLAYER_KILLZONE) {
		player_kill();
		return -1;
	}
		

	return 1;
}


void player_render() {
	if(!player)
		return;
	d_render_offset(-(player->x / 1000), -(player->y / 1000));
	shape_copy_render(player->shape);
	d_render_offset(0, 0);
	return;
}


void player_kill() {
	/*shape_copy_free(player->shape);
	free(player);
	player = NULL;
	*/
	return;
}
