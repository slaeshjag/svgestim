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
