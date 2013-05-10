#include "svgestim.h"
#include "map.h"

BULLET_LIST *bullet_add(BULLET_LIST *list, int x, int y, int angle, SHAPE *bullet) {
	BULLET_LIST *new;

	new = malloc(sizeof(BULLET_LIST));
	new->next = list;
	new->vel_x = (BULLET_VELOCITY * d_util_sin(angle + 900)) >> 16;
	new->vel_y = (BULLET_VELOCITY * d_util_sin(angle)) >> 16;
	new->x = x * 1000;
	new->y = y * 1000;
	new->life=0;
	new->copy = shape_copy_copy(bullet);
	return new;
}


void bullet_remove(BULLET_LIST **list, BULLET_LIST *remove) {
	
	for (; *list; list = &(*list)->next)
		if (*list == remove) {
			*list = (*list)->next;
			shape_copy_free(remove->copy);
			free(remove);
			return;
		}
	return;
}


void bullet_loop(BULLET_LIST **list_p) {
	BULLET_LIST **list, *l;
	ENEMY *enemy;
	
	for (list = list_p; *list;) {
		l=*list;
		d_render_offset(-(l->x / 1000), -(l->y / 1000));
		shape_copy_render(l->copy);
		l->x+=l->vel_x;
		l->y+=l->vel_y;

		l->life += d_last_frame_time();
		if (l->life >= BULLET_LIFE) {
			*list = l->next;
			shape_copy_free(l->copy);
			free(l);
			continue;
		}
		/* TODO: Test collision with all entities here */
		if((enemy=map_enemy_collide(l->copy, l->x, l->y))) {
			enemy->health-=10;
			*list = l->next;
			shape_copy_free(l->copy);
			free(l);
			continue;
		}
		
		list = &l->next;
	}
	d_render_offset(0, 0);
	return;
}

GRENADE_LIST *grenade_add(GRENADE_LIST *list, int x, int y, int angle, SHAPE *grenade) {
	GRENADE_LIST *new;

	new = malloc(sizeof(GRENADE_LIST));
	new->next = list;
	new->vel_x = (GRENADE_VELOCITY * d_util_sin(angle + 900)) >> 16;
	new->vel_y = (GRENADE_VELOCITY * d_util_sin(angle)) >> 16;
	new->x = x * 1000;
	new->y = y * 1000;
	new->life=0;
	new->copy = shape_copy_copy(grenade);
	return new;
}


void grenade_remove(GRENADE_LIST **list, GRENADE_LIST *remove) {
	
	for (; *list; list = &(*list)->next)
		if (*list == remove) {
			*list = (*list)->next;
			shape_copy_free(remove->copy);
			free(remove);
			return;
		}
	return;
}


void grenade_loop(GRENADE_LIST **list_p) {
	GRENADE_LIST **list, *l;
	ENEMY *enemy;
	d_render_tint(0x0, 0xFF, 0x0, 0xFF);
	for (list = list_p; *list;) {
		l=*list;
		d_render_offset(-(l->x / 1000), -(l->y / 1000));
		shape_copy_render(l->copy);
		l->x+=l->vel_x;
		l->y+=l->vel_y;
		l->vel_y+=256;
		
		if(map_collide(l->copy->coord, l->copy->lines, l->x / 1000, l->y / 1000))
			l->vel_y*=-1;
		
		l->life += d_last_frame_time();
		if (l->life >= GRENADE_LIFE) {
			if((enemy=map_enemy_collide(grenade_explosion, l->x, l->y))) {
				enemy->health-=70;
			}
			
			particle_emitter_new(200, 1000, 1, 3000, 255, 0, 0, PARTICLE_TYPE_PULSE, l->x/1000, l->y/1000, 50, 0, 3600);
			particle_emitter_new(200, 1000, 1, 3000, 255, 255, 0, PARTICLE_TYPE_PULSE, l->x/1000, l->y/1000, 50, 0, 3600);
			*list = l->next;
			shape_copy_free(l->copy);
			free(l);
			continue;
		}
		/* TODO: Test collision with all entities here */
		
		list = &l->next;
	}
	d_render_offset(0, 0);
	d_render_tint(0xFF, 0xFF, 0xFF, 0xFF);
	return;
}
