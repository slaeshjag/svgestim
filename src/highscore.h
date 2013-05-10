#ifndef __HIGHSCORE_H__
#define	__HIGHSCORE_H__

#define	HIGHSCORE_CAP		5


typedef struct {
	char			name[32];
	int			score;
} HIGHSCORE;

HIGHSCORE highscore_table[HIGHSCORE_CAP];


int highscore_init();
void highscore_save();
void highscore_add();
void highscore_render();
void highscore_handle(DARNIT_KEYS *keys, DARNIT_MOUSE *mouse);

#endif
