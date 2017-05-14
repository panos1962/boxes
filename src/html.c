#include "boxes.h"
#include <string.h>

static void blist_html(BLIST *);
static void vlist_html(BOX *);
static void hlist_html(BOX *);

static void box_html(BOX *box) {
	if (box->section)
	puts("<br/>");

	printf("<div class=\"box\"");

	if (box->color)
	printf(" style=\"background-color:%s;\"", box->color);

	puts(">");

	if (box->ilist) {
		puts("<table>");
		printf("<tbody>");

		if (box->vertical)
		vlist_html(box);

		else
		hlist_html(box);

		puts("</tbody>");
		puts("</table>");
	}

	else {
		if (box->title) {
			printf("<div class=\"header");

			if (box->font & FONT_BOLD)
			printf(" bold");

			if (box->font & FONT_ITALIC)
			printf(" italic");

			if (box->font & FONT_UNDERLINE)
			printf(" underline");

			printf("\">%s</div>\n", box->title);
		}
	}

	if (box->blist)
	blist_html(box->blist);

	puts("</div>");
}

static void hlist_html(BOX *box) {
	ILIST *p;

	if (box->title)
	printf(
		"<tr>"
		"<td colspan=\"100000\">"
		"<div class=\"header\">%s</div>"
		"</td>"
		"</tr>",
		box->title
	);

	puts("<tr>");
	for (p = box->ilist; p; p = p->nxt) {
		printf("<td");

		if (p->item->color)
		printf(" style=\"background-color:%s;\"", p->item->color);

		printf("><div class=\"vertical");

		if (!strcmp(p->item->s, "...")) {
			printf(" filler");
			free(p->item->s);
			p->item->s = "&hellip;";
		}

		printf("\">%s</div></td>\n", p->item->s);
	}
	puts("</tr>");
}

static void vlist_html(BOX *box) {
	ILIST *p;

	puts("<tr>");

	if (box->title)
	printf(
		"<td>"
		"<div class=\"vertical header\">%s</div>"
		"</td>",
		box->title
	);

	puts("<td>");
	for (p = box->ilist; p; p = p->nxt) {
		printf("<div class=\"horizontal");

		if (!strcmp(p->item->s, "...")) {
			printf(" filler");
			free(p->item->s);
			p->item->s = "&hellip;";
		}

		putchar('"');

		if (p->item->color)
		printf(" style=\"background-color:%s;\"", p->item->color);

		printf(">");

		printf("%s</div>", p->item->s);
	}
	puts("</td>");
}

static void blist_html(BLIST *blist) {
	if (!blist)
	return;

	box_html(blist->box);
	blist_html(blist->nxt);
}

void printer_html(BLIST *root, size_t level) {

	if (!root)
	return;

	puts(
	"<html>"
	"<head>"

	"<style>"
	".bold{font-weight:bold;text-shadow:1px 0px 0px #A0A0A0;}"
	".italic{font-family:italic;}"
	".underline{text-decoration:underline;}"
	"div{white-space:nowrap;}"
	"table{border-collapse:collapse;}"
	"td{padding:2px;}"
	"table,td{border:2px solid black;}"
	".box{display:inline-block;margin:10px;vertical-align:top;"
	"border-style:solid;border-width:2px}"
	".tbox{border-style:none;}"
	".header{text-align:center;padding:2px;text-shadow:1px 0px 0px #343434;}"
	".vertical{padding:2px;writing-mode:vertical-lr;transform:rotate(-180deg);vertical-align:bottom;}"
        ".horizontal{padding:2px;border-style:none none solid none;border-width:2px;}"
	".horizontal_td{padding:0;vertical-align:top;}"
	"</style>"

	"<script src=\"https://ajax.googleapis.com/ajax/libs/jquery/3.2.1/jquery.min.js\"></script>"

	"<script type=\"text/javascript\">"
	"\n//<![CDATA[\n"

	"$(document).ready(function() {"
		"$('table').each(function() {"
			"var list;"
			"var filler = undefined;"
			"var hmax = 0;"
			"var wone = 0;"
			"var wall = 0;"

			"$(this).parent().addClass('tbox');"

			"list = $(this).find('.vertical');"
			"if (list.length) {"

				"list.each(function() {"
					"var h = $(this).height();"

					"if (h > hmax)"
					"hmax = h;"

					"if ($(this).hasClass('filler')) {"
						"filler = $(this);"
						"wone = $(this).outerWidth(true);"
					"}"

					"wall += $(this).parent().innerWidth();"
				"});"

				"list.css('height', hmax + 'px');"

				"if (filler) {"
					"if ((wall / list.length) / wone > 1.2)"
					"filler.parent().css('width', (wall - (list.length * wone) + wone) + 'px');"
				"}"
			"}"

			"list = $(this).find('.horizontal');"
			"if (list.length) {"
				"list.each(function() {"
					"$(this).parent().addClass('horizontal_td');"
				"});"

				"$(list[list.length - 1]).css('border-style', 'none');"
			"}"
		"});"
	"});"

	"\n//]]>\n"
	"</script>"

	"</head>"
	"<body>"
	);

	blist_html(root);

	puts(
	"</body>"
	"</html>"
	);
}
