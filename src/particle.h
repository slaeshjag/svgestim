#ifndef __PARTICLE_H__
#define	__PARTICLE_H__


typedef struct {
	int			life;
	int			used;
	int			x;
	int			y;
	int			vel_x;
	int			vel_y;
} PARTICLE_ONE;


typedef enum {
	PARTICLE_TYPE_RADIAL_SHOWER,
	PARTICLE_TYPE_PULSE
} PARTICLE_TYPE;


typedef struct {
	int			max_spawnrate;
	int			speed_max;
	int			speed_min;
	int			life;
	int			max_particles;

	int			x;
	int			y;
	int			gravity;

	unsigned char		r;
	unsigned char		g;
	unsigned char		b;

	PARTICLE_ONE		*particle;
	DARNIT_POINT		*point;
	PARTICLE_TYPE		type;
} PARTICLE;


PARTICLE *particle_emitter_new(int max_particles, int life, int speed_min, int speed_max, unsigned char r, unsigned char g, unsigned char b, PARTICLE_TYPE type, int x, int y, int gravity);
void particle_emitter_loop(PARTICLE *particle);
PARTICLE *particle_emitter_free(PARTICLE *particle);

#endif
