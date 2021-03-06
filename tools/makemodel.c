#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <darnit/darnit.h>

#define ZOOM 4

int main(int argc, char **argv) {
	DARNIT_MOUSE mouse;
	DARNIT_KEYS keys;
	DARNIT_LINE *line;
	DARNIT_TILESHEET *mouse_ts;
	int x, y, w, h;
	int startx, starty, start_set=0;
	int lines;
	if(argc!=3)
		return 1;
	w=atoi(argv[1]);
	h=atoi(argv[2]);
	d_init_custom("blololol", w*ZOOM, h*ZOOM, 0, "trololo", NULL);
	mouse_ts=d_render_tilesheet_load("mouse.png", 16, 16, DARNIT_PFORMAT_RGB5A1);
	line=d_render_line_new(4096, 1*ZOOM);
	d_render_blend_enable();
	
	for(lines=0;;) {
		mouse=d_mouse_get();
		keys=d_keys_get();
		if(keys.start)
			break;
		if(keys.lmb) {
			if(!start_set) {
				x=startx=mouse.x;
				y=starty=mouse.y;
				start_set=1;
				printf("COORDINATES	%i %i", (mouse.x/ZOOM)-w/2, (mouse.y/ZOOM)-h/2);
				lines++;
			} else {
				putchar(' ');
				printf("%i %i", (mouse.x/ZOOM)-w/2, (mouse.y/ZOOM)-h/2);
				d_render_line_move(line, lines, x, y, mouse.x, mouse.y);
				x=mouse.x;
				y=mouse.y;
				lines++;
			}
		}
		if(start_set)
			d_render_line_move(line, lines, x, y, mouse.x, mouse.y);
		d_keys_set(keys);
		d_render_begin();
		if(start_set)
			d_render_line_draw(line, lines+1);
		d_render_tile_blit(mouse_ts, 0, mouse.x, mouse.y);
		d_render_end();
		d_loop();
	}
	printf(" %i %i\n", startx/ZOOM-w/2, starty/ZOOM-h/2);
	printf("LINES		%i\n", lines);
	printf("DURATION	200\n");
	
	d_quit();
	return 0;
}
