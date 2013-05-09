#ifndef SVGESTIM_H
#define SVGESTIM_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <darnit/darnit.h>

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
	GAMESTATE_QUIT,
	GAMESTATES,
} GAMESTATE;

struct {
	DARNIT_FONT *univox;
	DARNIT_FONT *vectroid;
} font;

DARNIT_PLATFORM platform;

void gamestate(GAMESTATE state);

#endif