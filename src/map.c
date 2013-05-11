#include "map.h"
#include "game.h"
#include "transform.h"
#include "player.h"
#include "enemy.h"
#include "shapeloader.h"

#define MAX(a, b) ((a)>(b)?(a):(b))
#define MIN(a, b) ((a)<(b)?(a):(b))
#define MAPS 1
#define MAP_LINES 1024
#define MAP_SECTIONS 128
#define MAP_SECTION_WIDTH 16
#define TILE_SIZE 8
#define ENEMIES_MAX 1024
#define	CAMERA_SCROLL_SPEED	48

static struct {
	unsigned char r;
	unsigned char g;
	unsigned char b;
} powerup_color[16]={
	{0xFF, 0x00, 0x00},
	{0x00, 0xFF, 0x00},
};

static DARNIT_TILESHEET *bogus_tilesheet;
static struct {
	unsigned int current;
	DARNIT_MAP *map[MAPS];
	DARNIT_LINE *line[MAP_SECTIONS];
	LINE line_coord[MAP_SECTIONS][MAP_LINES];
	unsigned int lines[MAP_SECTIONS];
	int sections;
	ENEMY *enemy[ENEMIES_MAX];
	unsigned int enemies;
	POWERUP powerup[50];
	int powerups;
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
	map.powerups=0;
	map.sections=-1;
	map.lines[0]=0;
	boss_shooting=0;
	for(x=0; x<map.map[i]->layer->tilemap->w; x++) {
		if(!(x%(MAP_SECTION_WIDTH))) {
			map.sections++;
			map.lines[map.sections]=0;
		}
		for(y=0; y<map.map[i]->layer->tilemap->h; y++) {
			switch((tmp=map.map[i]->layer->tilemap->data[y*map.map[i]->layer->tilemap->w+x])&0xF0) {
				case 0x30:
					map.enemy[map.enemies]=enemy_spawn(x*TILE_SIZE, y*TILE_SIZE, tmp&0xF, model.enemy[tmp&0xF]);
					if(tmp==0x30)
						map.enemy[map.enemies]->weapon.normal.right=shape_copy_copy(model.enemy_right);
					if(tmp==0x33)
						map.enemy[map.enemies]->health=600;
					map.enemies++;
					break;
				case 0x50:
					map.powerup[map.powerups].x=x*TILE_SIZE;
					map.powerup[map.powerups].y=y*TILE_SIZE;
					map.powerup[map.powerups].type=tmp&0xF;
					map.powerup[map.powerups].taken=0;
					map.powerup[map.powerups].shape=shape_copy_copy(model.powerup[tmp&0xF]);
					map.powerups++;
					break;
				default:
					break;
			}
			for(layer=1; layer<map.map[i]->layers; layer++) {
				switch(map.map[i]->layer[layer].tilemap->data[y*map.map[i]->layer[layer].tilemap->w+x]) {
					case 0x90:
						for(tmp=0; map.map[i]->layer[layer].tilemap->data[(y+tmp)*map.map[i]->layer[layer].tilemap->w+x]!=0xA0; tmp--);
						x2=TILE_SIZE*x;
						y2=TILE_SIZE*(y+tmp)+TILE_SIZE;
						d_render_line_move(map.line[map.sections], map.lines[map.sections], TILE_SIZE*x, TILE_SIZE*y+TILE_SIZE, x2, y2);
						break;
					case 0x91:
						for(tmp=0; map.map[i]->layer[layer].tilemap->data[y*map.map[i]->layer[layer].tilemap->w+x+tmp]!=0xA1; tmp++);
						x2=TILE_SIZE*(x+tmp)+TILE_SIZE;
						y2=TILE_SIZE*y;
						d_render_line_move(map.line[map.sections], map.lines[map.sections], TILE_SIZE*x+TILE_SIZE, TILE_SIZE*y+TILE_SIZE, x2, y2+TILE_SIZE);
						break;
					case 0x92:
						for(tmp=0; map.map[i]->layer[layer].tilemap->data[(y+tmp)*map.map[i]->layer[layer].tilemap->w+x-tmp]!=0xA2; tmp--);
						x2=TILE_SIZE*(x-tmp)+TILE_SIZE;
						y2=TILE_SIZE*(y+tmp)+TILE_SIZE;
						d_render_line_move(map.line[map.sections], map.lines[map.sections], TILE_SIZE*x+TILE_SIZE, TILE_SIZE*y+TILE_SIZE, x2, y2);
						break;
					case 0x93:
						for(tmp=0; map.map[i]->layer[layer].tilemap->data[(y+tmp)*map.map[i]->layer[layer].tilemap->w+x+tmp]!=0xA3; tmp++);
						x2=TILE_SIZE*(x+tmp)+TILE_SIZE;
						y2=TILE_SIZE*(y+tmp)+TILE_SIZE;
						d_render_line_move(map.line[map.sections], map.lines[map.sections], TILE_SIZE*x+TILE_SIZE, TILE_SIZE*y+TILE_SIZE, x2, y2);
						break;
					default:
						continue;
				}
				map.line_coord[map.sections][map.lines[map.sections]].x1=TILE_SIZE*x;
				map.line_coord[map.sections][map.lines[map.sections]].y1=TILE_SIZE*y;
				map.line_coord[map.sections][map.lines[map.sections]].x2=x2;//+TILE_SIZE/2;
				map.line_coord[map.sections][map.lines[map.sections]].y2=y2;//+TILE_SIZE/2;
				map.lines[map.sections]++;
			}
		}
	}
	map.current=i;
	player_spawn(64, 128, model.player, model.gun, model.grenade);
	camera_x = 0;
	camera_scroll_speed = CAMERA_SCROLL_SPEED;
}

void map_check_powerup(int x, int y) {
	int i;
	for(i=0; i<map.powerups; i++) {
		if((x>map.powerup[i].x-10)&&(x<map.powerup[i].x+10)&&(y>map.powerup[i].y-10)&&(y<map.powerup[i].y+10)&&((!map.powerup[i].taken))) {
			map.powerup[i].taken=1;
			switch(map.powerup[i].type) {
				case POWERUP_HP:
					d_sound_play(sound.powerup, 0, 127, 127, 0);
					player->health+=50;
					if(player->health>100)
						player->health=100;
					break;
				case POWERUP_GRENADES:
					d_sound_play(sound.powerup_small, 0, 127, 127, 0);
					player->grenades=(player->grenades+2);
				default:
					break;
			}
		}
	}
}

void map_cleanup() {
	int i;
	for(i=0; i<map.enemies; i++)
		if(map.enemy[i]->type==ENEMY_TYPE_NORMAL)
			map.enemy[i]->weapon.normal.right=shape_copy_free(map.enemy[i]->weapon.normal.right);
	map.enemies=0;
	
	for(i=0; i<map.powerups; i++) {
		shape_copy_free(map.powerup[i].shape);
	}
	map.powerups=0;
}

ENEMY *map_enemy_collide(SHAPE_COPY *shape, int x, int y) {
	int i;
	for(i=0; i<map.enemies; i++)
		if(map.enemy[i]->health>0&&shape_copy_collides(map.enemy[i]->shape, map.enemy[i]->x, map.enemy[i]->y, shape, x, y))
			return map.enemy[i];
	return NULL;
}

void map_loop() {
	int i;
	if(camera_scroll_speed>0) {
		camera_scroll_speed = CAMERA_SCROLL_SPEED + score / 5000;
		camera_x += camera_scroll_speed * d_last_frame_time();
	}
	if (camera_x / 1000 >= map.sections * TILE_SIZE * MAP_SECTION_WIDTH - 800) {
		//camera_x = map.sections * TILE_SIZE * MAP_SECTION_WIDTH;
		camera_scroll_speed=0;
	}
	for(i=0; i<map.enemies; i++) {
		enemy_move(map.enemy[i]);
	}
}

void map_render() {
	int i;
	d_render_offset(camera_x / 1000, 0);
	for(i=0; i<map.sections; i++)
		d_render_line_draw(map.line[i], map.lines[i]);
	
	for(i=0; i<map.enemies; i++)
		enemy_render(map.enemy[i]);
	
	for(i=0; i<map.powerups; i++) {
		d_render_tint(powerup_color[map.powerup[i].type].r, powerup_color[map.powerup[i].type].g, powerup_color[map.powerup[i].type].b, 0xFF);
		if(!map.powerup[i].taken) {
			d_render_offset(-(map.powerup[i].x) + camera_x / 1000, -(map.powerup[i].y));
			shape_copy_render(map.powerup[i].shape);
		}
	}
	d_render_tint(0xFF, 0xFF, 0xFF, 0xFF);
	d_render_offset(0, 0);
}

unsigned int map_get_tile(int x, int y, int layer) {
	return map.map[map.current]->layer[layer].tilemap->data[(y/TILE_SIZE)*map.map[map.current]->layer[layer].tilemap->w+x/TILE_SIZE];
}

MAP_SLOPE map_slope_direction(int dir, int section, int line) {
	dir*=-1;
	if (map.line_coord[section][line].x1 == map.line_coord[section][line].x2)
		return MAP_SLOPE_VERTICAL;
	
	if (dir < 0) {
		if (map.line_coord[section][line].y1 < map.line_coord[section][line].y2)
			return MAP_SLOPE_DOWN;
		if (map.line_coord[section][line].y1 > map.line_coord[section][line].y2)
			return MAP_SLOPE_UP;
		return MAP_SLOPE_NONE;
	} else {
		if (map.line_coord[section][line].y1 < map.line_coord[section][line].y2)
			return MAP_SLOPE_UP;
		if (map.line_coord[section][line].y1 > map.line_coord[section][line].y2)
			return MAP_SLOPE_DOWN;
		return MAP_SLOPE_NONE;
	}

	return MAP_SLOPE_NONE;
}

/*Mama mia, Italiano copy-pasta*/
/*Si, si, it's good for you*/
MAP_SLOPE map_collide_dir(int *obj, int lines, int x1, int y1, int dir) {
	unsigned int section1=~0, section2=0;
	int i;
	for(i=0; i<lines; i++) {
		section1=MIN(section1, obj[i*2] + x1);
		section2=MAX(section2, obj[i*2] + x1);
	}

	section1 /= (MAP_SECTION_WIDTH*TILE_SIZE);
	section2 /= (MAP_SECTION_WIDTH*TILE_SIZE);
	
	for(i=0; i<map.lines[section1]; i++) {
		if(map.line_coord[section1][i].x1 == map.line_coord[section1][i].x2)
			if(collision_test((void *) &map.line_coord[section1][i], 1, 0, 1, obj, lines, x1, y1))
				return map_slope_direction(dir, section1, i);
	}
	for(i=0; i<map.lines[section1]; i++) {
		if(collision_test((void *) &map.line_coord[section1][i], 1, 0, 0, obj, lines, x1, y1))
			return map_slope_direction(dir, section1, i);
	}
	if(section1==section2)
		return -1;
	for(i=0; i<map.lines[section2]; i++) {
		if(map.line_coord[section2][i].x1 == map.line_coord[section2][i].x2)
			if(collision_test((void *) &map.line_coord[section2][i], 1, 0, 1, obj, lines, x1, y1))
				return map_slope_direction(dir, section2, i);
	}
	for(i=0; i<map.lines[section2]; i++) {
		if(collision_test((void *) &map.line_coord[section2][i], 1, 0, 0, obj, lines, x1, y1))
			return map_slope_direction(dir, section2, i);
	}
	return -1;
}

int map_collide(int *obj, int lines, int x1, int y1) {
	unsigned int section1=0, section2=0;
	int i;
	for(i=0; i<lines; i++) {
		section1=MIN(section1, obj[i*2] + x1);
		section2=MAX(section2, obj[i*2] + x1);
	}

	section1 /= (MAP_SECTION_WIDTH*TILE_SIZE);
	section2 /= (MAP_SECTION_WIDTH*TILE_SIZE);
	
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
