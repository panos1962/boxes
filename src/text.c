#include "boxes.h"

static void print_indent(size_t level, char *s) {
	while (level-- > 0)
	putchar('\t');

	if (s)
	printf("%s", s);
}

static void print_ilist(ILIST *ilist) {
	char sep = ':';

	while (ilist) {
		printf("%c %s", sep, ilist->item->s);
		sep = ',';

		if (ilist->item->color)
		printf(" [%s]", ilist->item->color);
		
		ilist = ilist->nxt;
	}
}

static void print_box(BOX *box, size_t level) {
	print_indent(level, box->vertical ? "vbox" : "hbox");

	if (box->title)
	printf(", title = %s", box->title);

	if (box->color)
	printf(", box color = %s", box->color);

	print_ilist(box->ilist);
	putchar('\n');

	printer_text(box->blist, level + 1);
}

void printer_text(BLIST *root, size_t level) {
	while (root) {
		print_box(root->box, level);
		root = root->nxt;
	}
}
