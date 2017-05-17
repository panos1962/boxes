#include "boxes.h"

BOX *box_alloc(void) {
	BOX *p;

	if (!(p = malloc(sizeof(BOX))))
	fatal("box_alloc: out of memory", EXIT_MEMORY);

	p->title = NULL;
	p->font = 0;
	p->vertical = 0;
	p->color = NULL;
	p->section = 0;
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

// Εντοπίζουμε το τελευταίο box της λίστας και το καθιστούμε τελευταίο box
// της ενότητας στην οποία ανήκει, επομένως το επόμενο box θα εκτυπωθεί με
// line break.

BLIST *break_push(BLIST *l) {
	BLIST *p;

	if (!l)
	return l;

	for (p = l; p->nxt;)
	p = p->nxt;

	p->box->section = 1;
	return l;
}

// Η function "blist_push" δέχεται μια box list και την τοποθετεί στο τέλος της
// global box list "root".

BLIST *blist_push(BLIST *l) {
	BLIST *p;

	// Αν η "root" box list είναι κενή, τότε πρόκειται για την πρώτη box list,
	// επομένως καθίσταται "root".

	if (!root)
	return l;

	for (p = root; p->nxt;)
	p = p->nxt;

	p->nxt = l;
	return root;
}
