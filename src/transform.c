#include "svgestim.h"


void rotate_coord(int angle, int *x, int *y) {
	int x_o, y_o;

	x_o = *x;
	y_o = *y;

	*x = (d_util_sin(angle + 900) * x_o - d_util_sin(angle) * y_o) >> 16;
	*y = (d_util_sin(angle) * x_o + d_util_sin(angle + 900) * y_o) >> 16;

	return;
}


int collision_test(int *obj1, int lines_1, int x1, int y1, int *obj2, int lines_2, int x2, int y2) {
	int i, j, t1, t2, d, x1_, y1_, x2_, y2_, x3_, y3_, x4_, y4_;


	for (i = 0; i < lines_1; i++) {
		x1_ = obj1[i * 2] + x1;
		y1_ = obj1[i * 2 + 1] + y1;
		x2_ = obj1[i * 2 + 2] + x1;
		y2_ = obj1[i * 2 + 3] + y1;

		for (j = 0; j < lines_2; j++) {
			x3_ = obj2[j * 2] + x2;
			y3_ = obj2[j * 2 + 1] + y2;
			x4_ = obj2[j * 2 + 2] + x2;
			y4_ = obj2[j * 2 + 3] + y2;

			d = ((x2_ - x1_) * (y4_ - y3_)) - ((y2_ - y1_) * (x4_ - x3_));
			t1 = ((y1_ - y3_) * (x4_ - x3_)) - ((x1_ - x3_) * (y4_ - y3_));
			t1 <<= 16;
			t2 = ((y1_ - y3_) * (x2_ - x3_)) - ((x1_ - x3_) * (y2_ - y1_));
			t2 <<= 16;

			if (!d) return (!t1 && !t2);
			t1 /= d;
			t2 /= d;

			if (t1 >= 0 && t1 <= 32768 && t2 >= 0 && t2 <= 32768)
				return j + 1;
			
			continue;
		}
	}

	return 0;
}
