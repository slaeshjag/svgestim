#include "svgestim.h"
#include "map.h"

ENEMY *enemy_spawn(int x, int y, ENEMY_TYPE type, SHAPE *shape) {
	ENEMY *enemy;

	if (!(enemy = calloc(sizeof(ENEMY), 1)))
		return NULL;
	enemy->shape = shape_copy_copy(shape);
	enemy->x = x * 1000;
	enemy->y = y * 1000;
	enemy->type = type;
	enemy->health=100;
	
	return enemy;
}

void enemy_move(ENEMY *enemy) {
	if(enemy->health<=0) {
		if(enemy->type==ENEMY_TYPE_BOSS)
			player_kill();
		return;
	}
	//check if outside screeen
	if(camera_x+1000000<enemy->x)
		return;
	switch(enemy->type) {
		case ENEMY_TYPE_NORMAL:
			if(map_get_tile(enemy->x/1000+8*(1-2*enemy->weapon.normal.dir), (enemy->y+48)/1000, 0)==0x40) {
				enemy->weapon.normal.dir=!enemy->weapon.normal.dir;
				enemy->x+=16000*(1-2*enemy->weapon.normal.dir);
			}
			enemy->x+=1000*(1-2*enemy->weapon.normal.dir);
			break;
		case ENEMY_TYPE_BOSS:
			if((!(rand()%400))&&!boss_shooting) {
				boss_shooting=d_time_get()+4000;
				boss_emitter[0]=particle_emitter_new(700, 300, 1, 20000, 255, 0, 0, PARTICLE_TYPE_RADIAL_SHOWER, enemy->x/1000-128, enemy->y/1000-30, 0, 1700, 1900);
				boss_emitter[1]=particle_emitter_new(700, 300, 1, 20000, 255, 255, 0, PARTICLE_TYPE_RADIAL_SHOWER, enemy->x/1000-128, enemy->y/1000-30, 0, 1700, 1900);
			}
			if(!(rand()%50))
				enemy->weapon.bullet=bullet_add(enemy->weapon.bullet, enemy->x/1000-128, enemy->y/1000-10, 1700+(rand()%200), model.bullet, BULLET_OWNER_ENEMY);
			break;
		case ENEMY_TYPE_GUNMAN:
			if(!(rand()%100))
				enemy->weapon.bullet=bullet_add(enemy->weapon.bullet, enemy->x/1000-8, enemy->y/1000, 1700+(rand()%200), model.bullet, BULLET_OWNER_ENEMY);
			break;
		case ENEMY_TYPE_GRENADIER:
			if(!(rand()%100))
				enemy->weapon.grenade=grenade_add(enemy->weapon.grenade, enemy->x/1000-8, enemy->y/1000, 1800+(rand()%300), 500+(rand()%1500),model.grenade);
			break;
		default:
			break;
	}
}

void enemy_render(ENEMY *enemy) {
	SHAPE_COPY *shape=enemy->shape;
	if(enemy->health<=0)
		return;
	switch(enemy->type) {
		case ENEMY_TYPE_NORMAL:
			if(!enemy->weapon.normal.dir)
				shape=enemy->weapon.normal.right;
			break;
		case ENEMY_TYPE_BOSS:
		case ENEMY_TYPE_GUNMAN:
			if(gamestate_current()==GAMESTATE_GAME)
				bullet_loop(&enemy->weapon.bullet);
				break;
		case ENEMY_TYPE_GRENADIER:
			if(gamestate_current()==GAMESTATE_GAME)
				grenade_loop(&enemy->weapon.grenade);
				break;
		default:
			break;
	}
	d_render_offset(-(enemy->x/1000) + camera_x / 1000, -(enemy->y/1000));
	shape_copy_render(shape);
	d_render_offset(camera_x / 1000, 0);
}
