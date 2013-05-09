#ifndef __ENEMY_H__
#define	__ENEMY_H__

typedef struct {
	int			x;
	int			y;
	int			behaviour;

	SHAPE_COPY		*shape;
} ENEMY;

ENEMY *enemy_spawn(int x, int y, int behaviour, SHAPE *shape);


#endif
