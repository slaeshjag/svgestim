#include "svgestim.h"

ENEMY *enemy_spawn(int x, int y, int behaviour, SHAPE *shape) {
	ENEMY *enemy;

	if (!(enemy = malloc(sizeof(ENEMY))))
		return NULL;
	enemy->shape = shape_copy_copy(shape);
	enemy->x = x * 1000;
	enemy->y = y * 1000;
	enemy->behaviour = behaviour;
	enemy->health=100;
	
	return enemy;
}

void enemy_move(ENEMY *enemy) {
	if(enemy->health<=0)
		return;
}

void enemy_render(ENEMY *enemy) {
	if(enemy->health<=0)
		return;
	d_render_offset(-(enemy->x/1000), -(enemy->y/1000));
	shape_copy_render(enemy->shape);
	d_render_offset(0, 0);
}
