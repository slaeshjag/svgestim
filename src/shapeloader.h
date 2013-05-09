#ifndef __SHAPELOADER_H__
#define	__SHAPELOADER_H__


typedef struct {
	int		lines;
	int		*coordinate;
	int		spawn;
} SHAPE;


int shape_init(const char *fname);
SHAPE *shape_load(const char *name);
SHAPE *shape_free(SHAPE *shape);
void shape_cleanup();

#endif

#ifdef LOCAL_NAMESPACE

DARNIT_STRINGTABLE *shape_table;

#endif
