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
	int			particles_in_use;

	int			angle_min;
	int			angle_max;

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


typedef struct PARTICLE_LIST {
	struct PARTICLE_LIST	*next;
	PARTICLE		*p;
} PARTICLE_LIST;


PARTICLE *particle_emitter_new(int max_particles, int life, int speed_min, int speed_max, unsigned char r, unsigned char g, unsigned char b, PARTICLE_TYPE type, int x, int y, int gravity, int angle_min, int angle_max);
void particle_emitter_loop();
void particle_emitter_stop();

#endif
