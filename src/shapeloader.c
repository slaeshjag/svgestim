#include "svgestim.h"
#define	LOCAL_NAMESPACE
#include	"shapeloader.h"


int shape_init(const char *table) {
	shape_table = d_stringtable_open(table);

	return (shape_table != 0);
}



SHAPE *shape_load(const char *name) {
	SHAPE *shape;

	d_stringtable_section_load(shape_table, name);
	if (atoi(d_stringtable_entry(shape_table, "LINES")) <= 0) {
		d_stringtable_section_unload(shape_table, name);
		return NULL;
	}

	if (!(shape = malloc(sizeof(SHAPE))))
		return NULL;
	
	shape->lines = atoi(d_stringtable_entry(shape_table, "LINES"));
	shape->duration = atoi(d_stringtable_entry(shape_table, "DURATION"));
	shape->coordinate = malloc(sizeof(int) * (2 * shape->lines + 2));
	d_util_string_to_int_array(d_stringtable_entry(shape_table, "COORDINATES"), " ", shape->coordinate, shape->lines * 2 + 2);
	d_stringtable_section_unload(shape_table, name);

	return shape;
}


SHAPE *shape_free(SHAPE *shape) {
	free(shape->coordinate);
	free(shape);

	return NULL;
}


void shape_cleanup() {
	shape_table = d_stringtable_close(shape_table);

	return;
}


void shape_copy_rotate(SHAPE_COPY *copy, int angle) {
	int i;

	if (!copy)
		return;

	memcpy(copy->rot, copy->coord, sizeof(int) * (copy->lines * 2 + 2));
	i = 0;
	rotate_coord(angle, &copy->rot[i * 2], &copy->rot[i * 2 + 1]);
	for (; i < copy->lines; i++) {
		rotate_coord(angle, &copy->rot[i * 2 + 2], &copy->rot[i * 2 + 3]);
		if (i < copy->lines)
			d_render_line_move(copy->line, i, copy->rot[i * 2], copy->rot[i * 2 + 1], copy->rot[i * 2 + 2], copy->rot[i * 2 + 3]);
	}

	return;
}



SHAPE_COPY *shape_copy_copy(SHAPE *shape) {
	SHAPE_COPY *copy;

	if (!(copy = malloc(sizeof(SHAPE_COPY))))
		return NULL;
	
	copy->coord = memdup(shape->coordinate, (shape->lines + 1) * 2 * sizeof(int));
	copy->lines = shape->lines;
	copy->line = d_render_line_new(shape->lines, 1);
	copy->duration = shape->duration;
	copy->rot = malloc(sizeof(int) * (copy->lines * 2 + 2));
	shape_copy_rotate(copy, 0);

	return copy;
}

int shape_copy_collides(SHAPE_COPY *shape1, int x1, int y1, SHAPE_COPY *shape2, int x2, int y2) {
	int i;
	int buf[4];
	if(collision_test(shape1->rot, shape1->lines, x1/1000, y1/1000, shape2->rot, shape2->lines, x2/1000, y2/1000))
		return 1;
	buf[0]=shape1->rot[0];
	buf[1]=shape1->rot[1];
	for(i=1; i<shape1->lines; i++) {
		buf[2]=shape1->rot[i*2];
		buf[3]=shape1->rot[i*2+1];
		if(collision_test(buf, 1, x1/1000, y1/1000, shape2->rot, shape2->lines, x2/1000, y2/1000))
			return 1;
	}
	buf[0]=shape2->rot[0];
	buf[1]=shape2->rot[1];
	for(i=1; i<shape2->lines; i++) {
		buf[2]=shape2->rot[i*2];
		buf[3]=shape2->rot[i*2+1];
		if(collision_test(shape1->rot, shape1->lines, x1/1000, y1/1000, buf, 1, x2/1000, y2/1000))
			return 1;
	}
	return 0;
}

void shape_copy_render(SHAPE_COPY *copy) {
	if (!copy)
		return;
	d_render_line_draw(copy->line, ~0);

	return;
}


SHAPE_COPY *shape_copy_free(SHAPE_COPY *copy) {
	if (!copy)
		return NULL;
	d_render_line_free(copy->line);
	free(copy->coord);
	free(copy);

	return NULL;
}
