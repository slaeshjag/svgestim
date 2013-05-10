#ifndef __ENEMY_H__
#define	__ENEMY_H__

typedef enum {
	ENEMY_TYPE_NORMAL,
	ENEMY_TYPE_GUNMAN,
	ENEMY_TYPE_GRENADIER,
	ENEMY_TYPE_BOSS,
} ENEMY_TYPE;

typedef struct {
	int			x;
	int			y;
	ENEMY_TYPE			type;
	int			health;
	
	union {
		struct {
			int dir;
			SHAPE_COPY *right;
		} normal;
		BULLET_LIST *bullet;
		GRENADE_LIST *grenade;
	} weapon;
	
	SHAPE_COPY		*shape;
} ENEMY;

int boss_shooting;

ENEMY *enemy_spawn(int x, int y, ENEMY_TYPE type, SHAPE *shape);
void enemy_move(ENEMY *enemy);
void enemy_render(ENEMY *enemy);

#endif
