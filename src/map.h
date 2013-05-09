#ifndef MAP_H
#define MAP_H
#include "svgestim.h"
#include "game.h"

void map_init();
void map_load(int i);
void map_render();
int map_collide(int *obj, int lines, int x1, int y1);

#endif