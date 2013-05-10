#include "svgestim.h"

BULLET_LIST *bullet_add(BULLET_LIST *list, int x, int y, int angle, SHAPE *bullet) {
	BULLET_LIST *new;

	new = malloc(sizeof(BULLET_LIST));
	new->next = list;
	new->vel_x = (BULLET_VELOCITY * d_util_sin(angle + 900)) >> 16;
	new->vel_y = (BULLET_VELOCITY * d_util_sin(angle)) >> 16;
	new->x = x * 1000;
	new->y = y * 1000;
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
	BULLET_LIST *list, **orig;

	orig = list_p;
	list = *list_p;

	for (; list;) {
		d_render_offset(-(list->x / 1000), -(list->y / 1000));
		shape_copy_render(list->copy);

		list->life += d_last_frame_time();
		if (list->life >= BULLET_LIFE) {
			list_p = &(*list_p)->next;
			bullet_remove(orig, list);
			list = *list_p;
			continue;
		}
			
		list_p = &(*list_p)->next;
		list = *list_p;

		/* TODO: Test collision with all entities here */
	}	

	d_render_offset(0, 0);
	
	return;
}
