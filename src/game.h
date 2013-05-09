#ifndef GAME_H
#define GAME_H
#include "svgestim.h"

typedef struct {
	int x;
	int y;
} POINT;

typedef struct {
	int x1;
	int y1;
	int x2;
	int y2;
} RECTANGLE;

typedef RECTANGLE LINE;

void game_handle(DARNIT_KEYS *keys, DARNIT_MOUSE *mouse);
void game_render();

#endif