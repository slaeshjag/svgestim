#include "svgestim.h"
#include "highscore.h"


int highscore_init() {
	DARNIT_FILE *f;
	int i;
	char buff[64];

	highscore.enter_name = d_text_surface_new(font.vectroid, 600, 400, 150, 160);
	d_text_surface_string_append(highscore.enter_name, "New highscore! Enter your name");
	*highscore.name_buff = 0;
	highscore.name = d_menu_textinput_new(100, 200, font.vectroid, highscore.name_buff, 32, 400);

	for (i = 0; i < HIGHSCORE_CAP; i++) {
		highscore_table[i].score = 0;
		*highscore_table[i].name = 0;
	}
	d_text_surface_string_append(highscore.title = d_text_surface_new(font.univox, 64, 800, 16, 16), "highscores");
	highscore.text = d_text_surface_new(font.vectroid, 600, 800, 40, 150);
	if (!(f = d_file_open("highscore.txt", "rb"))) {
		fprintf(stderr, "Warning: Unable to open highscore table\n");
		return 0;
	}

	for (i = 0; i < HIGHSCORE_CAP && !d_file_eof(f); i++) {
		d_file_gets(buff, 64, f);
		sscanf(buff, "%i %s\n", &highscore_table[i].score, highscore_table[i].name);
	}

	return 1;
}


void highscore_save() {
	DARNIT_FILE *f;
	int i;
	char buff[64];

	if (!(f = d_file_open("highscore.txt", "wb"))) {
		fprintf(stderr, "Error: Unable to save highscore table\n");
		return;
	}

	for (i = 0; i < HIGHSCORE_CAP; i++) {
		if (highscore_table[i].score == 0)
			break;
		sprintf(buff, "%i %s\n", highscore_table[i].score, highscore_table[i].name);
		d_file_write(buff, strlen(buff), f);
	}
		d_file_close(f);

	return;
}


int highscore_is_new(int highscore_nn) {
	int i, j;

	j = 0;

	for (i = 0; i < HIGHSCORE_CAP; i++) {
		if (highscore_table[i].score < highscore_table[j].score)
			j = i;
	}

	if (highscore_table[j].score >= highscore_nn)
		return 0;
	
	return 1;
}


void highscore_add(int highscore_nn, char *name) {
	int i, j;
	HIGHSCORE hs;

	j = 0;
	for (i = 0; i < HIGHSCORE_CAP; i++) {
		if (highscore_table[i].score < highscore_table[j].score)  
			j = i;
	}

	if (highscore_table[i].score >= score)
		return;

	highscore_table[j].score = highscore_nn;
	sprintf(highscore_table[j].name, "%s", name);

	for (i = 0; i < HIGHSCORE_CAP; i++) {
		for (j = i; j > 0 && highscore_table[j].score > highscore_table[j - 1].score; j--) {
			hs = highscore_table[j];
			highscore_table[j] = highscore_table[j - 1];
			highscore_table[j - 1] = hs;
		}
	}

	highscore_save();

	return;
}


void highscore_render() {
	char buff[2048];
	int i;
	
	d_text_surface_draw(highscore.title);
	d_text_surface_reset(highscore.text);

	for (i = 0; i < HIGHSCORE_CAP; i++) {
		if(!highscore_table[i].score)
			break;
		sprintf(buff, "%s", highscore_table[i].name);
		d_text_surface_string_append(highscore.text, buff);
		sprintf(buff, "%.8i\n", highscore_table[i].score);
		d_text_surface_offset_next_set(highscore.text, 500);
		d_text_surface_string_append(highscore.text, buff);
	}
	d_text_surface_draw(highscore.text);
	
	return;
}


void highscore_handle(DARNIT_KEYS *keys, DARNIT_MOUSE *mouse) {
	if(keys->select||keys->start)
		gamestate(GAMESTATE_MENU);
	return;
}
