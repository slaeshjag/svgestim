#ifndef __BULLET_H__
#define	__BULLET_H__

#define	BULLET_VELOCITY		10000
#define	BULLET_LIFE		2000


typedef struct BULLET_LIST {
	SHAPE_COPY		*copy;
	int			x;
	int			y;
	int			vel_x;
	int			vel_y;
	int			life;
	struct BULLET_LIST	*next;
} BULLET_LIST;

#endif
