#include "svgestim.h"
#include "map.h"

#define	LOCAL_NAMESPACE
#define	PLAYER_HEALTH_BAR_HEIGHT	24
#include "player.h"


int player_spawn(int x, int y, SHAPE_SPRITE *shape, SHAPE *gun, SHAPE *grenade) {
	if (player)
		player_kill();

	if (!(player = malloc(sizeof(PLAYER)))) {
		fprintf(stderr, "Unable to alloc memory for player\n");
		return 0;
	}
	
	player->shape=shape;
	player->gun = shape_copy_copy(gun);
	player->grenade_shape = shape_copy_copy(grenade);

	player->x = x * 1000;
	player->y = y * 1000;
	player->vel_x = 0;
	player->vel_y = 0;
	player->health=100;
	player->grenades=3;
	
	player->gun_angle=0;
	player->bullet=NULL;
	player->grenade=NULL;
	player->healthbar = d_render_rect_new(1);
	player->scoretext = d_text_surface_new(font.vectroid, 32, 400, 400, 0);
	player->grenadestext = d_text_surface_new(font.vectroid, 32, 400, 400, 32);
	
	return 1;
}

void player_draw_stat() {
	char buff[32];

	sprintf(buff, "Score: %.8i\n", score);
	d_text_surface_reset(player->scoretext);
	d_text_surface_string_append(player->scoretext, buff);

	sprintf(buff, "Grenades: %.2i", player->grenades);
	d_text_surface_reset(player->grenadestext);
	d_text_surface_string_append(player->grenadestext, buff);

	d_render_rect_move(player->healthbar, 0, 0, 0, player->health * 2, PLAYER_HEALTH_BAR_HEIGHT);
	d_render_tint(255 - player->health * 255 / 100, player->health * 255 / 100, 0, 255);
	d_render_offset(0, 0);
	d_render_rect_draw(player->healthbar, 1);
	d_render_tint(255, 255, 255, 255);
	d_text_surface_draw(player->scoretext);
	d_text_surface_draw(player->grenadestext);

	return;
}

static int grenade_time=0;

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
	if(keys->b&&(!grenade_key)&&player->grenades)
		grenade_time=d_time_get();
	if((grenade_key&&!keys->b)||(grenade_time&&d_time_get()>=GRENADE_LIFE+grenade_time)) {
		int x=player->x/1000+28, y=player->y/1000+(player->gun_angle?player->gun_angle/20:0);
		if(player->grenades) {
			player->grenade=grenade_add(player->grenade, x, y, player->gun_angle, grenade_time+GRENADE_LIFE-d_time_get(), model.grenade);
			player->grenades--;
		}
		grenade_time=0;
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
	
	if (keys->l && !player->vel_y) {
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
			player->y-=50;
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

	if (player->x < camera_x + 32000)
		player->x = camera_x + 32000;
	if(player->vel_x) {
		shapesprite_animate(player->shape);
	}
	
	if(boss_shooting&&shape_copy_collides(flamethrower, 800-256-200, 480/2, shapesprite_get_current_shape(player->shape), player->x, player->y)) {
		player->health--;
	}
	
	map_check_powerup(player->x/1000+camera_x/1000, player->y);
	score++;

	return 1;
}


void player_render() {
	if(!player)
		return;
	
	shapesprite_render(player->shape, player->x / 1000 - camera_x / 1000, player->y / 1000);
	
	d_render_offset(-(player->x / 1000 - camera_x / 1000)-4, -(player->y / 1000)+6);
	shape_copy_rotate(player->gun, player->gun_angle);
	shape_copy_render(player->gun);
	shape_copy_rotate(player->gun, -player->gun_angle);
	d_render_offset(-((player->x/1000) - camera_x/1000 )-16, -(player->y/1000+(player->gun_angle?player->gun_angle/20:0)));
	
	if(grenade_time) {
		d_render_tint(0x0, 0xFF, 0x0, 0xFF);
		shape_copy_render(player->grenade_shape);
		d_render_tint(0xFF, 0xFF, 0xFF, 0xFF);
	}
	d_render_offset(0, 0);
	if(gamestate_current()!=GAMESTATE_GAME)
		return;
	bullet_loop(&player->bullet);
	grenade_loop(&player->grenade);
	player_draw_stat();
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
	d_render_rect_free(player->healthbar);
	d_text_surface_free(player->scoretext);
	d_text_surface_free(player->grenadestext);
	free(player);
	player = NULL;
	gamestate(GAMESTATE_GAMEOVER);

	return;
}
