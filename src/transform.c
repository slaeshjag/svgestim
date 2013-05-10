#include "svgestim.h"
#include "game.h"

void rotate_coord(int angle, int *x, int *y) {
	int x_o, y_o;

	x_o = *x;
	y_o = *y;

	*x = (d_util_sin(angle + 900) * x_o - d_util_sin(angle) * y_o) >> 16;
	*y = (d_util_sin(angle) * x_o + d_util_sin(angle + 900) * y_o) >> 16;

	return;
}

//////////////////////////////////////////////////////////////
// Intersects(): Uses basic linear algebra to determine if two 
//    integer-based line segments intersect.
// Input: a0,a1,b0,b1 -- 2D points, each with integers x and y.
// Returns: true if line segment a0-a1 intersects b0-b1.
// (c) Jake Askeland, January 2009, http://jake.askeland.ws/
static int _collision_test(POINT a0, POINT a1, POINT b0, POINT b1) {
 
	// detM = determinant of M, the matrix whose elements are
	// the coefficients of the parametric equations of lines
	// containing segments A and B.
	int detM = (a1.x - a0.x) * (b1.y - b0.y) 
			 - (b1.x - b0.x) * (a1.y - a0.y);
 
	// special case: A and B are parallel.
	// when A and B are parallel, 
	// detM is 0 and a bounds test is needed.
	if (detM == 0) {
 
		// special case: A and B are vertical line segments.
		if (a0.x == a1.x && b0.x == b1.x) {
			// true if A and B are in the same vertical line.
			if (a0.x == b0.x)
				// true when some bounds on Ay 
				// are in the bounds of By.
				return (b0.y <= a0.y && a0.y <= b1.y)
					   || (b0.y <= a1.y && a1.y <= b1.y);
 
			// different vertical lines, no intersection.
			else return 0;
		}
 
		// for parallel lines to overlap, they need the 
		// same y-intercept. integer relations to 
		// y-intercepts of A and B are as follows.
		int a_offset = ((a1.x-a0.x)*a0.y-(a1.y-a0.y)*a0.x) 
				   * (b1.x-b0.x);
		int b_offset = ((b1.x-b0.x)*b0.y-(b1.y-b0.y)*b0.x) 
				   * (a1.x-a0.x);
 
		// true only when A_y_intercept == B_y_intercept.
		if (b_offset == a_offset)
			// true when some bounds on ax 
			// are in the bounds of bx.
			return (b0.x <= a0.x && a0.x <= b1.x)
				   || (b0.x <= a1.x && a1.x <= b1.x);
 
		// different y intercepts; no intersection.
		else return 0;
 
	}
 
	// nMitc[0] = numerator_of_M_inverse_times_c0
	// nMitc[1] = numerator_of_M_inverse_times_c1
	int nMitc[2] = {
		(b0.x - a0.x) * (b1.y - b0.y) + 
		   (b0.y - a0.y) * (b0.x - b1.x),
		(b0.x - a0.x) * (a0.y - a1.y) + 
		   (b0.y - a0.y) * (a1.x - a0.x)
	};
 
	// true if an intersection between two non-parallel lines
	// occurs between the given segment points.
	return ((0 <= nMitc[0] && nMitc[0] <=  detM) 
			&& (0 >= nMitc[1] && nMitc[1] >= -detM)) ||
		   ((0 >= nMitc[0] && nMitc[0] >= detM) 
			&&   (0 <= nMitc[1] && nMitc[1] <= -detM)); 
}

int collision_test(int *obj1, int lines_1, int x1, int y1, int *obj2, int lines_2, int x2, int y2) {
	int i, j;
	POINT p1, p2, p3, p4;
	for (i = 0; i < lines_1; i++) {
		p1.x = obj1[i * 2] + x1;
		p1.y = obj1[i * 2 + 1] + y1;
		p2.x = obj1[i * 2 + 2] + x1;
		p2.y = obj1[i * 2 + 3] + y1;

		for (j = 0; j < lines_2; j++) {
			p3.x = obj2[j * 2] + x2;
			p3.y = obj2[j * 2 + 1] + y2;
			p4.x = obj2[j * 2 + 2] + x2;
			p4.y = obj2[j * 2 + 3] + y2;

			if(_collision_test(p1, p2, p3, p4))
				return 1;
		}
	}

	return 0;
}
