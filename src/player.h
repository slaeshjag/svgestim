#ifndef __PLAYER_H__
#define	__PLAYER_H__

typedef struct {
	int			x;
	int			y;
	int			vel_x;
	int			vel_y;
	int			gun_angle;
	int			health;
	
	SHAPE_SPRITE	*shape;
	SHAPE_COPY		*gun;
	BULLET_LIST		*bullet;
} PLAYER;



int player_spawn(int x, int y, SHAPE_SPRITE *shape, SHAPE *gun);
int player_loop(DARNIT_KEYS *keys);
void player_render();
void player_kill();

#endif

#ifdef LOCAL_NAMESPACE

PLAYER				*player;

#define	PLAYER_ACCELERATION	100000
#define	PLAYER_SPEED_X_MAX	200000
#define	PLAYER_SPEED_Y_MAX	200000
#define	PLAYER_JUMP_ACCELERATION	200000
#define	PLAYER_KILLZONE		800

#endif
