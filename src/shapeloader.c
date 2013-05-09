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
	shape->coordinate = malloc(sizeof(int) * 2 * shape->lines + 2);
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

	for (i = 0; i < copy->lines + 1; i++) {
		rotate_coord(angle, &copy->coord[i * 2], &copy->coord[i * 2 + 1]);
		if (i < copy->lines)
			d_render_line_move(copy->line, i, copy->coord[i * 2], copy->coord[i * 2 + 1], copy->coord[i * 2 + 2], copy->coord[i * 2 + 3]);
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
	shape_copy_rotate(copy, 0);

	return copy;
}


void shape_copy_render(SHAPE_COPY *copy) {
	d_render_line_draw(copy->line, ~0);

	return;
}


SHAPE_COPY *shape_copy_free(SHAPE_COPY *copy) {
	d_render_line_free(copy->line);
	free(copy->coord);
	free(copy);

	return NULL;
}
