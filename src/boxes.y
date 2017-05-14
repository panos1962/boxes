%{

#include "boxes.h"
#include <string.h>

%}

%union {
	BLIST *blist;
	BOX *box;
	ITEM *item;
	ILIST *ilist;
	TITLE *t;
	char *s;
	int i;
}

%start input

%type <blist> blist
%type <ilist> ilist
%type <item> item
%type <box> box
%type <i> orientation
%type <s> bcolor
%type <t> title
%type <s> color

%token <s> STRING
%token <s> COLOR

%token KWD_HBOX
%token KWD_VBOX
%token LBRACE1
%token RBRACE1
%token LBRACE2
%token RBRACE2

%%

input
	: blist {
		if (printer)
		printer($1, 0);
	}

	;

blist
	: {
		$$ = NULL;
	}

	| blist box {
		yyerrok;
		$$ = box_push($1, $2);
	}

	| blist ';' box {
		yyerrok;
		$3->section = 1;
		$$ = box_push($1, $3);
	}

	| blist error

	;

box
	: orientation bcolor '{' title ilist blist '}' {
		$$ = box_alloc();

		if ($4) {
			$$->title = $4->s;
			$$->font = $4->font;
		}

		$$->vertical = $1;
		$$->color = $2;

		if ($5)
		$$->ilist = $5;

		if ($6)
		$$->blist = $6;
	}
	;

orientation
	: {
		$$ = 0;
	}

	| KWD_HBOX {
		$$ = 0;
	}

	| KWD_VBOX {
		$$ = 1;
	}

	;

bcolor
	: {
		$$ = NULL;
	}

	| color {
		$$ = $1;
	}

	;

title
	: {
		$$ = NULL;
	}

	| LBRACE1 STRING RBRACE1 {
		$$ = title_alloc($2, 0);
	}

	| LBRACE2 STRING RBRACE2 {
		$$ = title_alloc($2, FONT_BOLD);
	}

	;

ilist
	: {
		$$ = NULL;
	}

	| item {
		yyerrok;
		$$ = ilist_alloc($1);
	}

	| ilist ',' item {
		yyerrok;
		$$ = item_push($1, $3);
	}

	| ilist ',' {
		yyerrok;
		$$ = $1;
	}

	| ilist ',' error {
		$$ = $1;
	}

	| ilist error {
		$$ = $1;
	}

	;

item
	: STRING {
		$$ = item_alloc($1, NULL);
	}

	| STRING color {
		$$ = item_alloc($1, $2);
	}

	| STRING error {
		$$ = item_alloc($1, NULL);
	}

	;

color
	: LBRACE1 COLOR RBRACE1 {
		$$ = $2;
	}

	;

%%
