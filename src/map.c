#include "map.h"

#define MAPS 1
#define MAP_LINES 8192
#define TILE_SIZE 8

static DARNIT_TILESHEET *bogus_tilesheet;
static struct {
	unsigned int current;
	DARNIT_MAP *map[MAPS];
	DARNIT_LINE *line;
	unsigned int lines;
} map;

void map_init() {
	int i;
	char fname[16];
	bogus_tilesheet=d_render_tilesheet_new(1, 1, 1, 1, DARNIT_PFORMAT_RGB5A1);
	for(i=0; i<=MAPS; i++) {
		sprintf(fname, "level%i.ldmz", i);
		map.map[i]=d_map_load(fname);
	}
	map.line=d_render_line_new(MAP_LINES, 1);
}

void map_load(int i) {
	int x, y, layer, tmp;
	map.lines=0;
	for(layer=1; layer<map.map[i]->layers; layer++)
		for(y=0; y<map.map[i]->layer[layer].tilemap->h; y++)
			for(x=0; x<map.map[i]->layer[layer].tilemap->w; x++) {
				switch(map.map[i]->layer[layer].tilemap->data[y*map.map[i]->layer[layer].tilemap->w+x]) {
					case 0x90:
						for(tmp=0; map.map[i]->layer[layer].tilemap->data[(y+tmp)*map.map[i]->layer[layer].tilemap->w+x]!=0xA0; tmp--);
						d_render_line_move(map.line, map.lines, TILE_SIZE*x+TILE_SIZE/2, TILE_SIZE*y+TILE_SIZE/2, TILE_SIZE*x+TILE_SIZE/2, TILE_SIZE*(y+tmp)+TILE_SIZE/2);
						map.lines++;
						break;
					case 0x91:
						for(tmp=0; map.map[i]->layer[layer].tilemap->data[y*map.map[i]->layer[layer].tilemap->w+x+tmp]!=0xA1; tmp++);
						d_render_line_move(map.line, map.lines, TILE_SIZE*x+TILE_SIZE/2, TILE_SIZE*y+TILE_SIZE/2, TILE_SIZE*(x+tmp)+TILE_SIZE/2, TILE_SIZE*y+TILE_SIZE/2);
						map.lines++;
						break;
					case 0x92:
						for(tmp=0; map.map[i]->layer[layer].tilemap->data[(y+tmp)*map.map[i]->layer[layer].tilemap->w+x-tmp]!=0xA2; tmp--);
						d_render_line_move(map.line, map.lines, TILE_SIZE*x+TILE_SIZE/2, TILE_SIZE*y+TILE_SIZE/2, TILE_SIZE*(x-tmp)+TILE_SIZE/2, TILE_SIZE*(y+tmp)+TILE_SIZE/2);
						map.lines++;
						break;
					case 0x93:
						for(tmp=0; map.map[i]->layer[layer].tilemap->data[(y+tmp)*map.map[i]->layer[layer].tilemap->w+x+tmp]!=0xA3; tmp++);
						d_render_line_move(map.line, map.lines, TILE_SIZE*x+TILE_SIZE/2, TILE_SIZE*y+TILE_SIZE/2, TILE_SIZE*(x+tmp)+TILE_SIZE/2, TILE_SIZE*(y+tmp)+TILE_SIZE/2);
						map.lines++;
						break;
				}
			}
	map.current=i;
}

void map_render() {
	d_render_line_draw(map.line, map.lines);
}