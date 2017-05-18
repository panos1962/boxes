#include "boxes.h"
#include <string.h>

static void blist_html(BLIST *, unsigned int);
static void vlist_html(BOX *);
static void hlist_html(BOX *);

static void box_html(BOX *box, unsigned int section) {
	if (section)
	puts("<br/>");

	if (box->ilist) {
		printf("<div class=\"box");

		if (box->vertical == BOX_WIDE)
		printf(" wbox");

		putchar('"');

		if (box->color)
		printf(" style=\"background-color:%s;\"", box->color);

		puts(">");

		puts("<table>");
		printf("<tbody>");

		if (box->vertical == BOX_VERTICAL)
		vlist_html(box);

		else
		hlist_html(box);

		puts("</tbody>");
		puts("</table>");
	}

	else {
		printf("<div class=\"box");

		switch (box->vertical) {
		case BOX_VERTICAL:
			printf(" vbox");
			break;
		case BOX_WIDE:
			printf(" wbox");
			break;
		}

		printf("\"");

		if (box->color)
		printf(" style=\"background-color:%s;\"", box->color);

		puts(">");

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
	blist_html(box->blist, 0);

	puts("</div>");
}

static void hlist_html(BOX *box) {
	ILIST *p;
	int n;

	for (n = 0, p = box->ilist; p; p = p->nxt)
	n++;

	if (box->title)
	printf(
		"<tr>"
		"<td colspan=\"%d\">"
		"<div class=\"header\">%s</div>"
		"</td>"
		"</tr>",
		n, box->title
	);

	puts("<tr class=\"ilist\">");
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

static void blist_html(BLIST *blist, unsigned int section) {
	if (!blist)
	return;

	box_html(blist->box, section);
	blist_html(blist->nxt, blist->box->section);
}

void printer_html(BLIST *root, size_t level) {

	if (!root)
	return;

	puts(
	"<!DOCTYPE html>"
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
	".box{position:relative;display:inline-block;margin:10px;vertical-align:top;"
	"border-style:solid;border-width:2px}"
	".wbox{display:block;}"
	".tbox{border-style:none;}"
	".header{text-align:center;padding:2px;text-shadow:1px 0px 0px #343434;}"
	".vertical{padding:2px;writing-mode:vertical-lr;vertical-align:bottom;"
	"transform:rotate(-180deg);-webkit-transform:rotate(-180deg);-ms-transform:rotate(-180deg);}"
        ".horizontal{padding:2px;border-style:none none solid none;border-width:2px;}"
	".horizontal_td{padding:0;vertical-align:top;}"
	".vcont{position:absolute;"
	"transform:rotate(-90deg);-webkit-transform:rotate(-90deg);-ms-transform:rotate(-90deg);}"
	".antivertical{transform:none;-webkit-transform:none;-ms-transform:none;}"
	"</style>"

	"<script src=\"https://ajax.googleapis.com/ajax/libs/jquery/3.2.1/jquery.min.js\"></script>"

	"<script type=\"text/javascript\">"
	"\n//<![CDATA[\n"

	"$(window).resize(function() {"
		"location.reload();"
	"});"

	"$(document).ready(function() {"
		"diamorfosi();"
	"});"

	"function diamorfosi() {"
		"$('table').each(function() {"
			"var list;"
			"var filler = undefined;"
			"var hmax = 0;"
			"var wone = 0;"
			"var wall = 0;"
			"var vblist;"
			"var vbi;"

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

					"return true;"
				"});"

				"list.css('height', hmax + 'px');"

				"if (filler) {"
					"if ((wall / list.length) / wone > 1.2)"
					"filler.parent().css('width', "
						"(wall - (list.length * wone) + wone) + 'px');"
				"}"
			"}"

			"list = $(this).find('.horizontal');"
			"if (list.length) {"
				"list.each(function() {"
					"$(this).parent().addClass('horizontal_td');"
				"});"

				"$(list[list.length - 1]).css('border-style', 'none');"

				"return true;"
			"}"

			"return true;"
		"});"

		"vblist = $('.vbox');"
		"for (vbi = vblist.length - 1; vbi >= 0; vbi--)"
		"vbox_render($(vblist[vbi]));"
	"}"

	"function vbox_render(box) {"
		"vb = true;"
		"var w = box.outerWidth(true);"
		"var w1 = box.width();"
		"var h = box.outerHeight(true);"
		"var h1 = box.height();"
		"var x = box.html();"
		"var y;"

		"box.removeClass('vbox');"
		"box.css({"
			"width: h + 'px',"
			"height: w + 'px',"
		"}).empty()."
		"append(y = $('<div>').addClass('vcont').html(x).css({"
			"top: ((w / 2) - (h1 / 2)) + 'px',"
			"left: ((h / 2) - (w1 / 2)) + 'px',"
		"}));"

		"y.find('.vertical').addClass('antivertical');"

		"box.find('.ilist').each(function() {"
			"var x = $(this).children();"
			"var i;"

			"$(this).empty();"
			"for (i = x.length - 1; i >= 0; i--)"
			"$(this).append(x.get(i));"

			"return true;"
		"});"
	"}"

	"\n//]]>\n"
	"</script>"

	"</head>"
	"<body>"
	);

	blist_html(root, 0);

	puts(
	"</body>"
	"</html>"
	);
}
