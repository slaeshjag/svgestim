#ifndef	__SHAPESPRITE_H__
#define	__SHAPESPRITE_H__

typedef struct {
	SHAPE_COPY		**frame;
	int			frames;
	int			frame_n;
	int			time;
} SHAPE_SPRITE;


SHAPE_SPRITE *shapesprite_new(const char *frame, int frames);
void shapesprite_animate(SHAPE_SPRITE *sprite);
SHAPE_COPY *shapesprite_get_current_shape(SHAPE_SPRITE *sprite);
void shapesprite_render(SHAPE_SPRITE *sprite, int x, int y);

#endif
