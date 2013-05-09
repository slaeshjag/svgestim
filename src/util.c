#include "svgestim.h"

void *memdup(void *src, size_t bytes) {
	void *dest;

	dest = malloc(bytes);
	memcpy(dest, src, bytes);

	return dest;
}


