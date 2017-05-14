#include "boxes.h"

TITLE *title_alloc(char *s, unsigned int font) {
	TITLE *p;

	if (!(p = malloc(sizeof(TITLE))))
	fatal("title_alloc: out of memory", EXIT_MEMORY);

	p->s = s;
	p->font = font;

	return p;
}
