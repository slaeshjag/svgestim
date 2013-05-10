#ifndef MAP_H
#define MAP_H
#include "svgestim.h"
#include "game.h"

void map_init();
void map_load(int i);
void map_loop();
void map_render();
ENEMY *map_enemy_collide(SHAPE_COPY *shape, int x, int y);
int map_collide(int *obj, int lines, int x1, int y1);

#endif
