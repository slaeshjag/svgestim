#ifndef __PLAYER_H__
#define	__PLAYER_H__

typedef struct {
	int			x;
	int			y;
	int			vel_x;
	int			vel_y;
	int			gun_angle;
	int			health;
	int			grenades;
	
	SHAPE_SPRITE	*shape;
	SHAPE_COPY		*gun;
	SHAPE_COPY		*grenade_shape;
	BULLET_LIST		*bullet;
	GRENADE_LIST		*grenade;
} PLAYER;



int player_spawn(int x, int y, SHAPE_SPRITE *shape, SHAPE *gun, SHAPE *grenade);
int player_loop(DARNIT_KEYS *keys);
void player_render();
void player_hurt(int damage);
void player_kill();

#endif

PLAYER				*player;

#ifdef LOCAL_NAMESPACE

#define	PLAYER_ACCELERATION	300000
#define	PLAYER_FRICTION	300000
#define	PLAYER_SPEED_X_MAX	200000
#define	PLAYER_SPEED_Y_MAX	200000
#define	PLAYER_JUMP_ACCELERATION	150000
#define	PLAYER_KILLZONE		480
#define	PLAYER_GRAVITY		256

#endif
