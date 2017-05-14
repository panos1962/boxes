#include "boxes.h"

ITEM *item_alloc(char *s, char *color) {
	ITEM *p;

	if (!(p = malloc(sizeof(ITEM))))
	fatal("item_alloc: out of memory", EXIT_MEMORY);

	p->s = s;
	p->color = color;

	return p;
}

ILIST *ilist_alloc(ITEM *item) {
	ILIST *p;

	if (!(p = malloc(sizeof(ILIST))))
	fatal("ilist_alloc: out of memory", EXIT_MEMORY);

	p->item = item;
	p->nxt = NULL;

	return p;
}

ILIST *item_push(ILIST *root, ITEM *item) {
	ILIST *p;
	ILIST *q;

	p = ilist_alloc(item);

	if (!root)
	return p;

	for (q = root; q->nxt;)
	q = q->nxt;

	q->nxt = p;

	return root;
}
