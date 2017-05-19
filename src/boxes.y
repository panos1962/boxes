%{

#include "boxes.h"

%}

%union {
	BLIST *blist;
	BOX *box;
	BATTRS *battrs;
	ILIST *ilist;
	ITEM *item;
	char *s;
	int i;
}

%start input

%type <blist> blist
%type <box> box
%type <battrs> battrs
%type <ilist> ilist
%type <item> item
%type <i> btype
%type <s> bcolor
%type <s> color

%token <s> STRING
%token <s> COLOR

%token KWD_HBOX		/* "hbox" */
%token KWD_VBOX		/* "vbox" */
%token KWD_WBOX		/* "wbox" */

%token LBRACE2		/* "[[" */
%token RBRACE2		/* "]]" */

%%

/*
Το input πρέπει να απεικονίζει μια ολοκληρωμένη box list (blist).
*/

input
	/*
	Επειδή υπάρχει περίπτωση να έχουμε πολλές box lists σε διάφορα
	input files, δεν προβαίνουμε απευθείας στην εκτύπωση του ανά
	χείρας box list, αλλά την προσθέτουμε σε τυχόν ήδη υπάρχουσες
	box lists.
	*/

	: blist {
		root = blist_push($1);
	}

	;

/*
Η δομή "blist" απεικονίζει μια σειρά από boxes τα οποία παρατίθενται
το ένα μετά το άλλο. Αυτά τα boxes θα εμφανιστούν inline χωρίς αλλαγή
γραμμής. Αν θέλουμε να κάνουμε αλλαγή γραμμής, πρέπει να παρεμβάλουμε
σχετικό τελεστή (";").
*/

blist
	: {
		$$ = NULL;
	}

	/*
	Παράθεση box δίπλα στα προηγούμενα χωρίς αλλαγή γραμμής.
	*/

	| blist box {
		yyerrok;
		$$ = box_push($1, $2);
	}

	/*
	Εκκίνηση νέας ενότητας με αλλαγή γραμμής για το επόμενο box.
	*/

	| blist ';' {
		$$ = break_push($1);
	}

	| blist error

	;

/*
Η δομή box είναι η σημαντικότερη δομή του input και απεικονίζει ένα box.
Κάθε box έχει προσανατολισμό (οριζόντιο/κάθετο), χρώμα, τίτλο και
περιεχόμενα. Τα περιεχόμενα μπορούν να είναι μια σειρά από subboxes,
ή μια σειρά από items εφόσον πρόκειται για «τελικό» box.
*/

box
	: battrs '{' blist '}' {
		$$ = box_alloc($1);
		$$->blist = $3;
	}

	| battrs '{' ilist '}' {
		$$ = box_alloc($1);
		$$->ilist = $3;
	}

	;

battrs
	: {
		if (!($$ = malloc(sizeof(BATTRS))))
		fatal("battrs_alloc: out of memory", EXIT_MEMORY);

		$$->type = BOX_UNDEFINED;
		$$->color = NULL;
		$$->title = NULL;
		$$->font = FONT_UNDEFINED;
	}

	| battrs btype {
		yyerrok;

		if ($$->type != BOX_UNDEFINED)
		error("box type redefinition");

		$$->type = $2;
	}

	| battrs bcolor {
		yyerrok;

		if ($$->color)
		error("box color redefinition");

		$$->color = $2;
	}

	| battrs '[' STRING ']' {
		yyerrok;

		if ($$->title)
		error("box title redefinition");

		$$->title = $3;
		$$->font = 0;
	}

	| battrs LBRACE2 STRING RBRACE2 {
		yyerrok;

		if ($$->title != BOX_UNDEFINED)
		error("box title redefinition");

		$$->title = $3;
		$$->font = FONT_BOLD;
	}

	| battrs error
	;

/*
Ακολουθεί ενότητα καθορισμού προσανοτολισμού/μεγέθους box.
*/

btype
	/*
	Μπορούμε, ωστόσο, να ορίσουμε ρητά ότι το box είναι
	«οριζόντιο» με το keyword "hbox".
	*/

	: KWD_HBOX {
		$$ = BOX_HORIZONTAL;
	}

	/*
	Το box καθορίζεται «κάθετο» με το keyword "vbox".
	*/

	| KWD_VBOX {
		$$ = BOX_VERTICAL;
	}

	/*
	Το box καθορίζεται «διάπλατο» σε όλο το πλάτος του πατρικού
	box (wide box) με το keyword "wbox".
	*/

	| KWD_WBOX {
		$$ = BOX_WIDE;
	}

	;

/*
Ακολουθεί ενότητα καθορισμού χρώματος box.
*/

bcolor
	: color {
		$$ = $1;
	}

	/*
	Αν υπάρχουν τα brackets αλλά δεν υπάρχει χρώμα ανάμεσα στα
	brackets, τότε το πρόγραμμα θα αντιλαμβάνεται ότι υπάρχει
	χρώμα, αλλά το χρώμα ως string θα είναι κενό. Αυτό μπορεί
	να χρησιμοποιηθεί για να «ακυρώσει» το περίγραμμα του box.
	*/

	| '<' '>' {
		$$ = "";
	}

	;

/*
Η δομή ilist απεικονίζει μια σειρά από items που περιέχονται σε «τελικό» box.
Πράγματι, ένα box μπορεί να περιέχει άλλα boxes, ή να είναι «τελικό» πράγμα
που σημαίνει ότι περιλαμβάνει μια σειρά από strings που θα εκτυπωθούν σε ένα
box εν είδει πίνακος.
*/

ilist
	: item {
		$$ = ilist_alloc($1);
	}

	/*
	Τα στοιχεία της ilist χωρίζονται μεταξύ τους με ","
	*/

	| ilist ',' item {
		yyerrok;
		$$ = item_push($1, $3);
	}

	/*
	Για λόγους ευκολίας σε τυχόν αλλαγές θέσης των items μιας ilist
	δίδεται η δυνατότητα τερματικού "," που σημαίνει ότι το "," στο
	τελευταίο στοιχείο της ilist δεν θεωρείται σφάλμα.
	*/

	| ilist ',' {
		yyerrok;
		$$ = $1;
	}

	| ilist error item {
		yyerrok;
		$$ = item_push($1, $3);
	}

	| ilist ',' error {
		yyerrok;
		$$ = $1;
	}

	| error {
		$$ = NULL;
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
	/*
	Το χρώμα καθορίζεται ως string ανάμεσα σε LT/GT brackets.
	*/

	: '<' COLOR '>' {
		$$ = $2;
	}

	;

%%
