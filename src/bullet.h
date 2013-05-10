#ifndef __BULLET_H__
#define	__BULLET_H__

#define	BULLET_VELOCITY		10000
#define	BULLET_LIFE		2000
#define	GRENADE_VELOCITY		10000
#define	GRENADE_LIFE		1000

typedef enum {
	BULLET_OWNER_PLAYER,
	BULLET_OWNER_ENEMY,
} BULLET_OWNER;

typedef struct BULLET_LIST {
	SHAPE_COPY		*copy;
	int			x;
	int			y;
	int			vel_x;
	int			vel_y;
	int			life;
	BULLET_OWNER owner;
	struct BULLET_LIST	*next;
} BULLET_LIST;

typedef struct GRENADE_LIST {
	SHAPE_COPY		*copy;
	int			x;
	int			y;
	int			vel_x;
	int			vel_y;
	int			life;
	struct GRENADE_LIST	*next;
} GRENADE_LIST;

SHAPE_COPY *grenade_explosion;
SHAPE_COPY *flamethrower;

BULLET_LIST *bullet_add(BULLET_LIST *list, int x, int y, int angle, SHAPE *bullet, BULLET_OWNER owner);
void bullet_remove(BULLET_LIST **list, BULLET_LIST *remove);
void bullet_loop(BULLET_LIST **list_p);

GRENADE_LIST *grenade_add(GRENADE_LIST *list, int x, int y, int angle, int time, SHAPE *grenade);
void grenade_remove(GRENADE_LIST **list, GRENADE_LIST *remove);
void grenade_loop(GRENADE_LIST **list_p);

#endif
