#include "boxes.h"
#include <string.h>

char *strsave(char *s) {
	char *p;

	if (!s)
	return NULL;

	if (!(p = malloc(strlen(s) + 1)))
	fatal("strsave: out of memory", EXIT_MEMORY);

	strcpy(p, s);
	return p;
}

char *strnsave(char *s, size_t l) {
	char *p;

	if (!s)
	return NULL;

	if (!(p = malloc(l + 1)))
	fatal("strnsave: out of memory", EXIT_MEMORY);

	strcpy(p, s);
	return p;
}
