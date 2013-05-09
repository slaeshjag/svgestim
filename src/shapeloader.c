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
