#ifndef SVGESTIM_H
#define SVGESTIM_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <darnit/darnit.h>

#include "util.h"
#include "shapeloader.h"
#include "shapesprite.h"
#include "bullet.h"
#include "player.h"
#include "transform.h"
#include "enemy.h"
#include "particle.h"
#include "highscore.h"

#define TEXT_STATIC(var, font, text, x, y) (d_text_surface_string_append((var)=d_text_surface_new((font), 64, 800, (x), (y)), (text)))
#define TEXT_INDENT(var, text, x) \
	d_text_surface_offset_next_set((var), (x));\
	d_text_surface_string_append((var), (text))

typedef enum {
	GAMESTATE_MENU,
	GAMESTATE_HIGHSCORE,
	GAMESTATE_INSTRUCTIONS,
	GAMESTATE_GAME,
	GAMESTATE_PAUSE,
	GAMESTATE_GAMEOVER,
	GAMESTATE_QUIT,
	GAMESTATES,
} GAMESTATE;

struct {
	DARNIT_FONT *univox;
	DARNIT_FONT *vectroid;
} font;

struct {
	SHAPE_SPRITE *player;
	SHAPE *gun;
	SHAPE *bullet;
	SHAPE *grenade;
	SHAPE *enemy[16];
	SHAPE *enemy_right;
	SHAPE *explosion;
	SHAPE *flamethrower;
	SHAPE *powerup[16];
} model;

struct {
	DARNIT_SOUND *explosion;
	DARNIT_SOUND *jump;
	DARNIT_SOUND *shoot;
	DARNIT_SOUND *enemy_kill;
	DARNIT_SOUND *powerup;
	DARNIT_SOUND *powerup_small;
	DARNIT_SOUND *music;
} sound;

DARNIT_PLATFORM platform;
int score;

void gamestate(GAMESTATE state);
GAMESTATE gamestate_current();

#endif
