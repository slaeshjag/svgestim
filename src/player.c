#include "svgestim.h"
#include "map.h"

#define	LOCAL_NAMESPACE
#include "player.h"


int player_spawn(int x, int y, SHAPE *shape, SHAPE *gun) {
	if (player)
		player_kill();

	if (!(player = malloc(sizeof(PLAYER)))) {
		fprintf(stderr, "Unable to alloc memory for player\n");
		return 0;
	}

	player->shape = shape_copy_copy(shape);
	player->gun = shape_copy_copy(gun);

	player->x = x * 1000;
	player->y = y * 1000;
	player->vel_x = 0;
	player->vel_y = 0;
	
	return 1;
}


int player_loop(DARNIT_KEYS *keys) {
	if (!player)
		return 0;
	
	if(keys->up)
		player->aim=AIM_UP;
	else if(keys->down)
		player->aim=AIM_DOWN;
	else
		player->aim=AIM_NORMAL;
	
	if (keys->left)
		player->vel_x += PLAYER_ACCELERATION * d_last_frame_time();
	else if (keys->right)
		player->vel_x -= PLAYER_ACCELERATION * d_last_frame_time();
	else if (player->vel_x)
		player->vel_x += (player->vel_x < 0 ? 1 : -1) * (PLAYER_ACCELERATION>>1) * d_last_frame_time();
	if (abs(player->vel_x) > PLAYER_SPEED_X_MAX)
		player->vel_x = (player->vel_x < 0 ? -1 : 1) * PLAYER_SPEED_X_MAX;
	
	player->x += player->vel_x * d_last_frame_time() * (keys->l ? 2 : 1);
	player->y += player->vel_y * d_last_frame_time();

	if (!map_collide(player->shape->coord, player->shape->lines, player->x / 1000, player->y / 1000))
		player->y += 32 * d_last_frame_time();

	if (player->x / 1000 >= PLAYER_KILLZONE) {
		player_kill();
		return -1;
	}
		

	return 1;
}


void player_render() {
	int gun_angle=0;
	if(!player)
		return;
	
	if(player->aim==AIM_UP)
		gun_angle=-450;
	else if(player->aim==AIM_DOWN)
		gun_angle=450;
	d_render_offset(-(player->x / 1000), -(player->y / 1000));
	shape_copy_render(player->shape);
	
	d_render_offset(-(player->x / 1000)-4, -(player->y / 1000)+6);
	shape_copy_rotate(player->gun, gun_angle);
	shape_copy_render(player->gun);
	shape_copy_rotate(player->gun, -gun_angle);
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
