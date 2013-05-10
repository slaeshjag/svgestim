#ifndef __SHAPELOADER_H__
#define	__SHAPELOADER_H__



typedef struct {
	int		lines;
	int		*coordinate;
	int		spawn;
	int		duration;
} SHAPE;


typedef struct {
	int		lines;
	int		*coord;
	int		min;
	int		max;
	int		duration;
	int		*rot;
	DARNIT_LINE	*line;
} SHAPE_COPY;


int shape_init(const char *fname);
SHAPE *shape_load(const char *name);
SHAPE *shape_free(SHAPE *shape);
void shape_cleanup();

void shape_copy_rotate(SHAPE_COPY *copy, int angle);
SHAPE_COPY *shape_copy_copy(SHAPE *shape);
void shape_copy_render(SHAPE_COPY *copy);
int shape_collides(SHAPE *shape1, int x1, int y1, SHAPE *shape2, int x2, int y2);
SHAPE_COPY *shape_copy_free(SHAPE_COPY *copy);

#endif

#ifdef LOCAL_NAMESPACE

DARNIT_STRINGTABLE *shape_table;

#endif
