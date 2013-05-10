#include "svgestim.h"
#include "map.h"

#define	LOCAL_NAMESPACE
#include "player.h"


int player_spawn(int x, int y, SHAPE_SPRITE *shape, SHAPE *gun) {
	if (player)
		player_kill();

	if (!(player = malloc(sizeof(PLAYER)))) {
		fprintf(stderr, "Unable to alloc memory for player\n");
		return 0;
	}
	
	player->shape=shape;
	player->gun = shape_copy_copy(gun);

	player->x = x * 1000;
	player->y = y * 1000;
	player->vel_x = 0;
	player->vel_y = 0;
	player->health=100;
	player->grenades=3;
	
	player->gun_angle=0;
	player->bullet=NULL;
	player->grenade=NULL;
	
	return 1;
}


/* Bästa Snyggkoden™ i stan! */
int player_loop(DARNIT_KEYS *keys) {
	SHAPE_COPY *shape;
	DARNIT_KEYS set;
	static int shoot_key=0;
	static int grenade_key=0;
	int dir;
	if (!player)
		return 0;
	shape=shapesprite_get_current_shape(player->shape);
	
	if(keys->up)
		player->gun_angle=-200;
	else if(keys->down)
		player->gun_angle=200;
	else
		player->gun_angle=0;
	
	if(keys->a&&!shoot_key) {
		int x=player->x/1000+28, y=player->y/1000+(player->gun_angle?player->gun_angle/20:0);
		player->bullet=bullet_add(player->bullet, x, y, player->gun_angle, model.bullet, BULLET_OWNER_PLAYER);
		d_sound_play(sound.shoot, 0, 127, 127, 0);
		particle_emitter_new(30, 100, 8000, 10000, 255, 0, 0, PARTICLE_TYPE_PULSE, x, y, 0, player->gun_angle-100, player->gun_angle+100);
		particle_emitter_new(30, 100, 8000, 10000, 255, 255, 0, PARTICLE_TYPE_PULSE, x, y, 0, player->gun_angle-100, player->gun_angle+100);
	}
	if(keys->b&&!grenade_key&&player->grenades) {
		//TODO: hold grenade while holding key, throw on release for timed attacks
		int x=player->x/1000+28, y=player->y/1000+(player->gun_angle?player->gun_angle/20:0);
		player->grenade=grenade_add(player->grenade, x, y, player->gun_angle, model.grenade);
		player->grenades--;
	}
	shoot_key=keys->a;
	grenade_key=keys->b;
	
	if (keys->left)
		player->vel_x -= (PLAYER_ACCELERATION * d_last_frame_time()) / 1000;
	else if (keys->right)
		player->vel_x += (PLAYER_ACCELERATION * d_last_frame_time()) / 1000;
	else if (player->vel_x) {
		if (abs(player->vel_x) < (PLAYER_ACCELERATION * d_last_frame_time()) / 1000)
			player->vel_x = 0;
		else
			player->vel_x += ((player->vel_x < 0 ? 1 : -1) * (PLAYER_FRICTION) * d_last_frame_time()) / 1000;
	}
	if (abs(player->vel_x) > PLAYER_SPEED_X_MAX)
		player->vel_x = (player->vel_x < 0 ? -1 : 1) * PLAYER_SPEED_X_MAX;
	
	if (keys->l) {
		set = d_keys_zero();
		set.l = 1;
		d_keys_set(set);
		player->vel_y -= PLAYER_JUMP_ACCELERATION;
	}

	player->vel_y += PLAYER_GRAVITY * d_last_frame_time();

	if (abs(player->vel_y) > PLAYER_SPEED_Y_MAX)
		player->vel_y = (player->vel_y < 0 ? -1 : 1) * PLAYER_SPEED_Y_MAX;

	dir=map_collide_dir(shape->coord, shape->lines, player->x / 1000, player->y / 1000, player->vel_x);
	if(dir==MAP_SLOPE_UP) {
		player->x += player->vel_x * d_last_frame_time()/ 1000;
		player->y-=abs(player->vel_x) * d_last_frame_time() / 1000;
	} else if(dir==MAP_SLOPE_VERTICAL) {
		player->x-=player->vel_x/ 200;
		player->vel_x*=-1;
		player->y+=2000;
	} else if(dir!=MAP_SLOPE_VERTICAL) {
		if (dir != -1)
			player->y-50;
		player->x += player->vel_x * d_last_frame_time() / 1000;
	}
	if (player->x < 32000)
		player->x = 32000;
	
	if (!map_collide(shape->coord, shape->lines, player->x / 1000, player->y / 1000) || player->vel_y < 0)
		player->y += player->vel_y * d_last_frame_time() / 1000;
	else {
		player->vel_y = 0;
	}

	if (player->y / 1000 >= PLAYER_KILLZONE||map_enemy_collide(shapesprite_get_current_shape(player->shape), player->x, player->y)) {
		player_kill();
		return -1;
	}
	if(player->vel_x) {
		shapesprite_animate(player->shape);
	}

	return 1;
}


void player_render() {
	if(!player)
		return;
	
	shapesprite_render(player->shape, player->x / 1000, player->y / 1000);
	
	d_render_offset(-(player->x / 1000)-4, -(player->y / 1000)+6);
	shape_copy_rotate(player->gun, player->gun_angle);
	shape_copy_render(player->gun);
	shape_copy_rotate(player->gun, -player->gun_angle);
	d_render_offset(0, 0);
	if(gamestate_current()!=GAMESTATE_GAME)
		return;
	bullet_loop(&player->bullet);
	grenade_loop(&player->grenade);
	return;
}

void player_hurt(int damage) {
	player->health-=damage;
	printf("hp %i\n", player->health);
	if(player->health<=0)
		player_kill();
}

void player_kill() {
	while(player->bullet)
		bullet_remove(&player->bullet, player->bullet);
	shape_copy_free(player->gun);
	free(player);
	player = NULL;
	gamestate(GAMESTATE_GAMEOVER);
	return;
}
