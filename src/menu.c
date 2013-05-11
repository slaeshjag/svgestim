#include "menu.h"
#include "game.h"
#include "svgestim.h"

#define TEXT_MENU(i, text) \
	TEXT_STATIC(menu.menu[(i)-1], font.vectroid, "F"#i, 128+64, 128+32*((i)-1)); \
	TEXT_INDENT(menu.menu[(i)-1], (text), 64)

#define TEXT_INSTRUCTIONS(i, key, text) \
	TEXT_STATIC(instructions.keys[(i)-1], font.vectroid, (key), 128+64, 128+32*((i)-1)); \
	TEXT_INDENT(instructions.keys[(i)-1], (text), 128)

static struct {
	DARNIT_TEXT_SURFACE *title;
	DARNIT_TEXT_SURFACE *menu[4];
} menu;

static struct {
	DARNIT_TEXT_SURFACE *title;
	DARNIT_TEXT_SURFACE *subtitle;
	DARNIT_TEXT_SURFACE *score;
} gameover;

static struct {
	DARNIT_TEXT_SURFACE *title;
} pause;

static struct {
	DARNIT_TEXT_SURFACE *title;
	DARNIT_TEXT_SURFACE *keys[4];
} instructions;

void menu_init() {
	TEXT_STATIC(menu.title, font.univox, "svgestim", 64, 16);
	TEXT_MENU(1, "start game");
	TEXT_MENU(2, "highscores");
	TEXT_MENU(3, "instructions");
	TEXT_MENU(4, "quit");
	
	TEXT_STATIC(gameover.title, font.univox, "game over,", 16, 16);
	TEXT_STATIC(gameover.subtitle, font.univox, "n00b", 64+32, 64);
	gameover.score=d_text_surface_new(font.vectroid, 64, 800, 128, 128);
	
	TEXT_STATIC(pause.title, font.univox, "game paused", 8, 16);
	
	TEXT_STATIC(instructions.title, font.univox, "instruction", 8, 16);
	TEXT_INSTRUCTIONS(1, "arrow keys:", "move and aim");
	TEXT_INSTRUCTIONS(2, "z:", "jump");
	TEXT_INSTRUCTIONS(3, "x:", "shoot");
	TEXT_INSTRUCTIONS(4, "c:", "throw grenade");
	TEXT_INSTRUCTIONS(5, "esc:", "quit to menu");
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

void gameover_handle(DARNIT_KEYS *keys, DARNIT_MOUSE *mouse) {
	if (highscore_is_new(score)) {
		d_render_begin();
		if (d_menu_loop(highscore.name) != -1) {
			if (*highscore.name_buff < 0x20)
				sprintf(highscore.name_buff, "arne.");
			highscore_add(score, highscore.name_buff);
			gamestate(GAMESTATE_HIGHSCORE);
		}
		d_render_end();
	}

	else if(keys->start)
		gamestate(GAMESTATE_MENU);
}

void gameover_render() {
	static char scoretext[64];
	d_text_surface_draw(gameover.title);
	d_text_surface_draw(gameover.subtitle);
	d_text_surface_reset(gameover.score);
	sprintf(scoretext, "Score %i", score);
	d_text_surface_string_append(gameover.score, scoretext);
	if (highscore_is_new(score))
		d_text_surface_draw(highscore.enter_name);
	
	d_text_surface_draw(gameover.score);
}

void pause_handle(DARNIT_KEYS *keys, DARNIT_MOUSE *mouse) {
	if(keys->select)
		gamestate(GAMESTATE_MENU);
	if(keys->start)
		gamestate(GAMESTATE_GAME);
}

void pause_render() {
	game_render();
	d_text_surface_draw(pause.title);
}

void instructions_handle(DARNIT_KEYS *keys, DARNIT_MOUSE *mouse) {
	if(keys->select||keys->start)
		gamestate(GAMESTATE_MENU);
}

void instructions_render() {
	int i;
	d_text_surface_draw(instructions.title);
	for(i=0; i<5; i++)
		d_text_surface_draw(instructions.keys[i]);
}
