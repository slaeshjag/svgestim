#ifndef MAP_H
#define MAP_H
#include "svgestim.h"
#include "game.h"

int			camera_x;
int			camera_scroll_speed;

typedef enum {
	MAP_SLOPE_NONE,
	MAP_SLOPE_UP,
	MAP_SLOPE_DOWN,
	MAP_SLOPE_VERTICAL
} MAP_SLOPE;


void map_init();
void map_load(int i);
void map_loop();
void map_render();
ENEMY *map_enemy_collide(SHAPE_COPY *shape, int x, int y);
MAP_SLOPE map_collide_dir(int *obj, int lines, int x1, int y1, int dir);
int map_collide(int *obj, int lines, int x1, int y1);

#endif
