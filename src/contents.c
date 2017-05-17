#include "boxes.h"

CONTENTS *contents_alloc(void *l, unsigned int type) {
	CONTENTS *p;

	if (!(p = malloc(sizeof(CONTENTS))))
	fatal("contents_alloc: out of memory", EXIT_MEMORY);

	switch (type) {
	case CONTENTS_BLIST:
		p->list.b = (BLIST *)l;
		break;
	case CONTENTS_ILIST:
		p->list.i = (ILIST *)l;
		break;
	default:
		fatal("contents_alloc: invalid type", EXIT_ERROR);
		break;
	}

	p->type = type;
	return p;
}
