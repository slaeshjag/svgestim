#ifndef __PLAYER_H__
#define	__PLAYER_H__

typedef struct {
	int			x;
	int			y;
	int			vel_x;
	int			vel_y;

	SHAPE_COPY		*shape;
} PLAYER;



int player_spawn(int x, int y, SHAPE *shape);
int player_loop();
void player_render();
void player_kill();

#endif

#ifdef LOCAL_NAMESPACE

PLAYER				*player;

#define	PLAYER_ACCELERATION	100000
#define	PLAYER_SPEED_X_MAX	200000
#define	PLAYER_KILLZONE		800

#endif
