#include "svgestim.h"
#include "highscore.h"


int highscore_init() {
	DARNIT_FILE *f;
	int i;
	char buff[64];

	for (i = 0; i < HIGHSCORE_CAP; i++) {
		highscore_table[i].score = 0;
		*highscore_table[i].name = 0;
	}

	if (!(f = d_file_open("highscore.txt", "rb"))) {
		fprintf(stderr, "Warning: Unable to open highscore table\n");
		return 0;
	}

	d_file_gets(buff, 64, f);
	for (i = 0; i < HIGHSCORE_CAP && !d_file_eof(f); i++) {
		sscanf(buff, "%i %s\n", &highscore_table[i].score, highscore_table[i].name);
	}

	highscore_text = d_text_surface_new(font.vectroid, 600, 800, 40, 150);

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
		d_file_close(f);
	}

	return;
}


void highscore_add(int highscore, char *name) {
	int i, j;
	j = 0;
	HIGHSCORE hs;

	for (i = 0; i < HIGHSCORE_CAP; i++) {
		if (highscore_table[i].score < highscore_table[j].score)
			j = i;
	}

	highscore_table[j].score = highscore;
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

	d_text_surface_reset(highscore_text);

	for (i = 0; i < HIGHSCORE_CAP; i++) {
		sprintf(buff, "%s", highscore_table[i].name);
		d_text_surface_string_append(highscore_text, buff);
		sprintf(buff, "%.8i\n", highscore_table[i].score);
		d_text_surface_offset_next_set(highscore_text, 500);
		d_text_surface_string_append(highscore_text, buff);

	}

	return;
}


void highscore_handle(DARNIT_KEYS *keys, DARNIT_MOUSE *mouse) {
	return;
}
