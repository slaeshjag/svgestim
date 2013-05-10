#include "map.h"
#include "game.h"
#include "transform.h"
#include "player.h"
#include "enemy.h"

#define MAX(a, b) ((a)>(b)?(a):(b))
#define MIN(a, b) ((a)<(b)?(a):(b))
#define MAPS 1
#define MAP_LINES 1024
#define MAP_SECTIONS 128
#define MAP_SECTION_WIDTH (8*32)
#define TILE_SIZE 8
#define ENEMIES_MAX 1024

static DARNIT_TILESHEET *bogus_tilesheet;
static struct {
	unsigned int current;
	DARNIT_MAP *map[MAPS];
	DARNIT_LINE *line[MAP_SECTIONS];
	LINE line_coord[MAP_SECTIONS][MAP_LINES];
	unsigned int lines[MAP_SECTIONS];
	unsigned int sections;
	ENEMY *enemy[ENEMIES_MAX];
	unsigned int enemies;
} map;

void map_init() {
	int i;
	char fname[16];
	bogus_tilesheet=d_render_tilesheet_new(1, 1, 1, 1, DARNIT_PFORMAT_RGB5A1);
	for(i=0; i<=MAPS; i++) {
		sprintf(fname, "level%i.ldmz", i);
		map.map[i]=d_map_load(fname);
	}
	for(i=0; i<MAP_SECTIONS; i++)
		map.line[i]=d_render_line_new(MAP_LINES, 1);
}

void map_load(int i) {
	int x, y, layer, tmp, x2, y2;
	map.sections=0;
	map.lines[0]=0;
	
	for(x=0; x<map.map[i]->layer->tilemap->w; x++) {
		for(y=0; y<map.map[i]->layer->tilemap->h; y++) {
			switch(map.map[i]->layer->tilemap->data[y*map.map[i]->layer->tilemap->w+x]) {
				case 0x30:
					map.enemy[map.enemies]=enemy_spawn(x*TILE_SIZE, y*TILE_SIZE, 0, model.enemy);
					map.enemies++;
					break;
				default:
					break;
			}
			for(layer=1; layer<map.map[i]->layers; layer++) {
				switch(map.map[i]->layer[layer].tilemap->data[y*map.map[i]->layer[layer].tilemap->w+x]) {
					case 0x90:
						for(tmp=0; map.map[i]->layer[layer].tilemap->data[(y+tmp)*map.map[i]->layer[layer].tilemap->w+x]!=0xA0; tmp--);
						x2=TILE_SIZE*x+TILE_SIZE/2;
						y2=TILE_SIZE*(y+tmp)+TILE_SIZE/2;
						break;
					case 0x91:
						for(tmp=0; map.map[i]->layer[layer].tilemap->data[y*map.map[i]->layer[layer].tilemap->w+x+tmp]!=0xA1; tmp++);
						x2=TILE_SIZE*(x+tmp)+TILE_SIZE/2;
						y2=TILE_SIZE*y+TILE_SIZE/2;
						break;
					case 0x92:
						for(tmp=0; map.map[i]->layer[layer].tilemap->data[(y+tmp)*map.map[i]->layer[layer].tilemap->w+x-tmp]!=0xA2; tmp--);
						x2=TILE_SIZE*(x-tmp)+TILE_SIZE/2;
						y2=TILE_SIZE*(y+tmp)+TILE_SIZE/2;
						break;
					case 0x93:
						for(tmp=0; map.map[i]->layer[layer].tilemap->data[(y+tmp)*map.map[i]->layer[layer].tilemap->w+x+tmp]!=0xA3; tmp++);
						x2=TILE_SIZE*(x+tmp)+TILE_SIZE/2;
						y2=TILE_SIZE*(y+tmp)+TILE_SIZE/2;
						break;
					default:
						continue;
				}
				d_render_line_move(map.line[map.sections], map.lines[map.sections], TILE_SIZE*x+TILE_SIZE/2, TILE_SIZE*y+TILE_SIZE/2, x2, y2);
				map.line_coord[map.sections][map.lines[map.sections]].x1=TILE_SIZE*x+TILE_SIZE/2;
				map.line_coord[map.sections][map.lines[map.sections]].y1=TILE_SIZE*y+TILE_SIZE/2;
				map.line_coord[map.sections][map.lines[map.sections]].x2=x2;
				map.line_coord[map.sections][map.lines[map.sections]].y2=y2;
				map.lines[map.sections]++;
			}
		}
		if(!(x%(MAP_SECTION_WIDTH))) {
			map.sections++;
			map.lines[map.sections]=0;
		}
	}
	map.current=i;
	player_spawn(64, 128, model.player, model.gun);
}

void map_loop() {
	int i;
	for(i=0; i<map.enemies; i++) {
		enemy_move(map.enemy[i]);
	}
}

void map_render() {
	int i;
	for(i=0; i<map.sections; i++)
		d_render_line_draw(map.line[i], map.lines[i]);
	
	for(i=0; i<map.enemies; i++)
		enemy_render(map.enemy[i]);
}

int map_collide(int *obj, int lines, int x1, int y1) {
	unsigned int section1=0, section2=0;
	int i;
	for(i=0; i<lines; i++) {
		section1=MIN(section1, obj[i*2] + x1);
		section2=MAX(section2, obj[i*2] + x1);
	}

	section1 /= MAP_SECTION_WIDTH;
	section2 /= MAP_SECTION_WIDTH;
	
	for(i=0; i<map.lines[section1]; i++) {
		if(collision_test((void *) &map.line_coord[section1][i], 1, 0, 0, obj, lines, x1, y1))
			return 1;
	}
	if(section1==section2)
		return 0;
	for(i=0; i<map.lines[section2]; i++) {
		if(collision_test((void *) &map.line_coord[section2][i], 1, 0, 0, obj, lines, x1, y1))
			return 1;
	}
	return 0;
}
