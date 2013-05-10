#include "svgestim.h"

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
	if(enemy->health<=0)
		return;
	//check if outside screeen
	switch(enemy->type) {
		case ENEMY_TYPE_GUNMAN:
			if(!(rand()%100))
				enemy->weapon.bullet=bullet_add(enemy->weapon.bullet, enemy->x/1000-8, enemy->y/1000, 1700+(rand()%200), model.bullet, BULLET_OWNER_ENEMY);
		default:
			break;
	}
}

void enemy_render(ENEMY *enemy) {
	if(enemy->health<=0)
		return;
	d_render_offset(-(enemy->x/1000), -(enemy->y/1000));
	shape_copy_render(enemy->shape);
	d_render_offset(0, 0);
	switch(enemy->type) {
		case ENEMY_TYPE_GUNMAN:
			if(gamestate_current()==GAMESTATE_GAME)
				bullet_loop(&enemy->weapon.bullet);
		default:
			break;
	}
}
