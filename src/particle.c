#include "svgestim.h"

static PARTICLE_LIST *p_list = NULL;


static void particle_list_add(PARTICLE *p) {
	struct PARTICLE_LIST *l = malloc(sizeof(PARTICLE_LIST));
	l->next = p_list;
	l->p = p;

	p_list = l;

	return;
}


static void particle_list_remove(PARTICLE *p) {
	struct PARTICLE_LIST **l, *tmp;
	
	for (l = &p_list; *l; l = &(*l)->next)
		if ((*l)->p == p) {
			tmp = (*l);
			*l = (*l)->next;
			free(tmp);
			return;
		}
	return;
}


PARTICLE *particle_emitter_new(int max_particles, int life, int speed_min, int speed_max, unsigned char r, unsigned char g, unsigned char b, PARTICLE_TYPE type, int x, int y, int gravity, int angle_min, int angle_max) {
	PARTICLE *p;
	int i;

	if (!(p = malloc(sizeof(PARTICLE))))
		return p;
	if (!(p->particle = malloc(sizeof(PARTICLE_ONE) * max_particles))) {
		free(p);
		return p;
	}

	for (i = 0; i < max_particles; i++)
		p->particle[i].used = 0;
	p->max_particles = max_particles;
	if (type == PARTICLE_TYPE_PULSE)
		p->max_spawnrate = ~0;
	else
		p->max_spawnrate = max_particles * 1000 / life;
	p->speed_max = speed_max;
	p->speed_min = speed_min;
	p->point = d_render_point_new(max_particles, 1);
	p->life = life;
	p->particles_in_use = 0;

	p->angle_min = angle_min;
	p->angle_max = angle_max;
	p->r = r;
	p->g = g;
	p->b = b;
	p->gravity = gravity;
	p->x = x * 1000;
	p->y = y * 1000;

	particle_list_add(p);

	return p;
}


static void particle_spawn(PARTICLE *particle, int index) {
	int angle, speed;

	angle = rand() % (particle->angle_max - particle->angle_min) + particle->angle_min;
	speed = rand() % (particle->speed_max - particle->speed_min) + particle->speed_min;
	particle->particle[index].vel_x = (d_util_sin(angle + 900) * speed) >> 16;
	particle->particle[index].vel_y = (d_util_sin(angle) * speed) >> 16;
	particle->particle[index].used = 1;
	particle->particle[index].life = 0;
	particle->particle[index].x = particle->x;
	particle->particle[index].y = particle->y;
	particle->particles_in_use++;

	return;
}


static void particle_move(PARTICLE *particle, int index) {
	particle->particle[index].x += particle->particle[index].vel_x;
	particle->particle[index].y += particle->particle[index].vel_y;
	particle->particle[index].vel_y += particle->gravity;
	if (!(particle->particle[index].used = ((particle->particle[index].life += d_last_frame_time()) < particle->life))) {
		d_render_point_move(particle->point, index, ~0, ~0);
		particle->particles_in_use--;
	} else 
		d_render_point_move(particle->point, index, particle->particle[index].x / 1000, particle->particle[index].y / 1000);

	return;
}


static int particle_emitter_loop_i(PARTICLE *particle) {
	unsigned int i, spawn, spawned;

	spawn = (particle->max_spawnrate == ~0) ? ~0 :  particle->max_spawnrate * d_last_frame_time() / 1000;
	if (spawn == ~0)
		particle->max_spawnrate = 0;
	spawned = 0;

	for (i = 0; i < particle->max_particles; i++)
		if (!particle->particle[i].used && spawn > spawned) {
			particle_spawn(particle, i);
			spawned++;
		} else if (particle->particle[i].used)
			particle_move(particle, i);
	
	d_render_tint(particle->r, particle->g, particle->b, 255);
	d_render_point_draw(particle->point, ~0);
	d_render_tint(255, 255, 255, 255);
	return particle->particles_in_use;
}


PARTICLE *particle_emitter_free(PARTICLE *particle) {
	free(particle->particle);
	d_render_point_free(particle->point);
	free(particle);

	return NULL;
}


void particle_emitter_stop(PARTICLE *particle) {
	particle->max_spawnrate = 0;

	return;
}


void particle_emitter_loop() {
	struct PARTICLE_LIST *l, *next;
	PARTICLE *tmp;

	
	for (l = p_list; l; ) {
		if (!particle_emitter_loop_i(l->p)) {
			next = l->next;
			tmp = l->p;
			particle_list_remove(l->p);
			particle_emitter_free(tmp);
			l = next;
			continue;
		}

		l = l->next;
	}

	return;
}
