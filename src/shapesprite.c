#include "svgestim.h"


/* Lösningen Snyggve™ */
SHAPE_SPRITE *shapesprite_new(const char *frame, int frames) {
	int i;
	char buff[128];
	SHAPE_SPRITE *sprite = malloc(sizeof(SHAPE_SPRITE));
	SHAPE *tmp;
	
	sprite->frame = malloc(sizeof(SHAPE_SPRITE *) * frames);
	sprite->frames = frames;
	sprite->frame_n = 0;
	sprite->time = 0;

	for (i = 0; i < frames; i++) {
		sscanf(frame, "%s\n", buff);
		tmp = shape_load(buff);
		sprite->frame[i] = shape_copy_copy(tmp);
		shape_free(tmp);
		/* Stabilt som ett korthus, smart som att servera soppa i en voffla */
		frame = strstr(frame, " ") + 1;
	}

	return sprite;
}


void shapesprite_animate(SHAPE_SPRITE *sprite) {
	for (;;) {
		sprite->time += d_last_frame_time();
		if (sprite->frame[sprite->frame_n]->duration <= sprite->time) {
			if (sprite->frame[sprite->frame_n]->duration) {
				sprite->time -= sprite->frame[sprite->frame_n]->duration;
				sprite->frame_n++;
				if (sprite->frame_n == sprite->frames)
					sprite->frame_n = 0;
			} else
				break;
		} else
			break;
	}

	return;
}


SHAPE_COPY *shapesprite_get_current_shape(SHAPE_SPRITE *sprite) {
	return sprite->frame[sprite->frame_n];
}


void shapesprite_render(SHAPE_SPRITE *sprite, int x, int y) {
	d_render_offset(-x, -y);
	shape_copy_render(sprite->frame[sprite->frame_n]);
	d_render_offset(0, 0);

	return;
}
