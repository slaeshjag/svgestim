#ifndef __HIGHSCORE_H__
#define	__HIGHSCORE_H__

#define	HIGHSCORE_CAP		5

typedef struct {
	DARNIT_TEXT_SURFACE *title;
	DARNIT_TEXT_SURFACE *text;
	DARNIT_MENU         *name;
	DARNIT_TEXT_SURFACE *enter_name;
	char		    name_buff[128];
} HIGHSCORE_STORAGE;

typedef struct {
	char			name[32];
	int			score;
} HIGHSCORE;

HIGHSCORE highscore_table[HIGHSCORE_CAP];
HIGHSCORE_STORAGE highscore;


int highscore_init();
void highscore_save();
void highscore_add(int highscore, char *name);
int highscore_is_new();
void highscore_render();
void highscore_handle(DARNIT_KEYS *keys, DARNIT_MOUSE *mouse);

#endif
