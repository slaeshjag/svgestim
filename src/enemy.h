#ifndef __ENEMY_H__
#define	__ENEMY_H__

typedef struct {
	int			x;
	int			y;
	int			behaviour;
	int			health;

	SHAPE_COPY		*shape;
} ENEMY;

ENEMY *enemy_spawn(int x, int y, int behaviour, SHAPE *shape);
void enemy_move(ENEMY *enemy);
void enemy_render(ENEMY *enemy);

#endif
