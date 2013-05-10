#ifndef MENU_H
#define MENU_H
#include "svgestim.h"

void menu_init();
void menu_handle(DARNIT_KEYS *keys, DARNIT_MOUSE *mouse);
void menu_render();

void gameover_handle(DARNIT_KEYS *keys, DARNIT_MOUSE *mouse);
void gameover_render();

#endif
