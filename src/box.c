#include "boxes.h"

BOX *box_alloc(void) {
	BOX *p;

	if (!(p = malloc(sizeof(BOX))))
	fatal("box_alloc: out of memory", EXIT_MEMORY);

	p->title = NULL;
	p->font = 0;
	p->vertical = 0;
	p->color = NULL;
	p->blist = NULL;
	p->ilist = NULL;

	return p;
}

BLIST *blist_alloc(BOX *b) {
	BLIST *p;

	if (!(p = malloc(sizeof(BLIST))))
	fatal("blist_alloc: out of memory", EXIT_MEMORY);

	p->box = b;
	p->nxt = NULL;

	return p;
}

BLIST *box_push(BLIST *l, BOX *b) {
	BLIST *p;
	BLIST *q;

	p = blist_alloc(b);

	if (!l)
	return p;

	for (q = l; q->nxt;)
	q = q->nxt;

	q->nxt = p;
	return l;
}
