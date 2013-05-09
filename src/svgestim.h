#ifndef SVGESTIM_H
#define SVGESTIM_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <darnit/darnit.h>

typedef enum {
	GAMESTATE_MENU,
	GAMESTATE_GAME,
	GAMESTATE_PAUSE,
	GAMESTATE_QUIT,
	GAMESTATES,
} GAMESTATE;

DARNIT_FONT *univox;

void gamestate(GAMESTATE state);

#endif