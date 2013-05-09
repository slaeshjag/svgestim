#include "svgestim.h"

ENEMY *enemy_spawn(int x, int y, int behaviour, SHAPE *shape) {
	ENEMY *enemy;

	if (!(enemy = malloc(sizeof(ENEMY))))
		return NULL;
	enemy->shape = shape_copy_copy(shape);
	enemy->x = x * 1000;
	enemy->y = y * 1000;
	enemy->behaviour = behaviour;

	return enemy;
}
