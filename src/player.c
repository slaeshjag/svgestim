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
	
	player->gun_angle=0;
	player->bullet=NULL;
	
	return 1;
}


int player_loop(DARNIT_KEYS *keys) {
	static int shoot_key=0;
	if (!player)
		return 0;
	
	if(keys->up)
		player->gun_angle=-450;
	else if(keys->down)
		player->gun_angle=450;
	else
		player->gun_angle=0;
	
	if(keys->a&&!shoot_key) {
		player->bullet=bullet_add(player->bullet, player->x/1000, player->y/1000, player->gun_angle, model.bullet);
		particle_emitter_new(30, 100, 8000, 10000, 255, 0, 0, PARTICLE_TYPE_PULSE, player->x/1000, player->y/1000, 0, player->gun_angle-100, player->gun_angle+100);
		particle_emitter_new(30, 100, 8000, 10000, 255, 255, 0, PARTICLE_TYPE_PULSE, player->x/1000, player->y/1000, 0, player->gun_angle-100, player->gun_angle+100);
	}
	shoot_key=keys->a;
	
	if (keys->left)
		player->vel_x -= (PLAYER_ACCELERATION * d_last_frame_time()) / 1000;
	else if (keys->right)
		player->vel_x += (PLAYER_ACCELERATION * d_last_frame_time()) / 1000;
	else if (player->vel_x) {
		if (abs(player->vel_x) < (PLAYER_ACCELERATION * d_last_frame_time()) / 1000)
			player->vel_x = 0;
		else
			player->vel_x += ((player->vel_x < 0 ? 1 : -1) * (PLAYER_ACCELERATION) * d_last_frame_time()) / 1000;
	}
	if (abs(player->vel_x) > PLAYER_SPEED_X_MAX)
		player->vel_x = (player->vel_x < 0 ? -1 : 1) * PLAYER_SPEED_X_MAX;
	
	player->x += player->vel_x * d_last_frame_time() * (keys->l ? 2 : 1) / 1000;
	player->y += player->vel_y * d_last_frame_time() / 1000;

	if (!map_collide(player->shape->coord, player->shape->lines, player->x / 1000, player->y / 1000))
		player->y += 64 * d_last_frame_time();

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
	
	d_render_offset(-(player->x / 1000)-4, -(player->y / 1000)+6);
	shape_copy_rotate(player->gun, player->gun_angle);
	shape_copy_render(player->gun);
	shape_copy_rotate(player->gun, -player->gun_angle);
	d_render_offset(0, 0);
	bullet_loop(&player->bullet);
	return;
}


void player_kill() {
	/*shape_copy_free(player->shape);
	free(player);
	player = NULL;
	*/
	return;
}
