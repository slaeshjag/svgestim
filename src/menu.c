#include "menu.h"

#define TEXT_MENU(i, text) \
	TEXT_STATIC(menu.menu[(i)-1], font.vectroid, "F"#i, 128+64, 128+32*((i)-1)); \
	TEXT_INDENT(menu.menu[(i)-1], (text), 64)

static struct {
	DARNIT_TEXT_SURFACE *title;
	DARNIT_TEXT_SURFACE *menu[4];
} menu;

void menu_init() {
	TEXT_STATIC(menu.title, font.univox, "svgestim", 64, 16);
	TEXT_MENU(1, "start game");
	TEXT_MENU(2, "highscores");
	TEXT_MENU(3, "instructions");
	TEXT_MENU(4, "quit");
}

void menu_handle(DARNIT_KEYS *keys, DARNIT_MOUSE *mouse) {
	if(keys->a)
		gamestate(GAMESTATE_GAME);
	else if(keys->b)
		gamestate(GAMESTATE_HIGHSCORE);
	else if(keys->x)
		gamestate(GAMESTATE_INSTRUCTIONS);
	else if(keys->y)
		gamestate(GAMESTATE_QUIT);
}

void menu_render() {
	int i;
	d_text_surface_draw(menu.title);
	for(i=0; i<4; i++)
		d_text_surface_draw(menu.menu[i]);
}
