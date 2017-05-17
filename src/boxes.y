%{

#include "boxes.h"

%}

%union {
	BLIST *blist;
	BOX *box;
	TITLE *t;
	CONTENTS *c;
	ILIST *ilist;
	ITEM *item;
	char *s;
	int i;
}

%start input

%type <blist> blist
%type <box> box
%type <t> title
%type <c> contents
%type <ilist> ilist
%type <item> item
%type <i> orientation
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
	: orientation bcolor '{' title contents '}' {
		$$ = box_alloc();

		if ($4) {
			$$->title = $4->s;
			$$->font = $4->font;
		}

		$$->vertical = $1;
		$$->color = $2;

		switch ($5->type) {
		case CONTENTS_BLIST:
			$$->blist = $5->list.b;
			$$->ilist = NULL;
			break;
		case CONTENTS_ILIST:
			$$->ilist = $5->list.i;
			$$->blist = NULL;
			break;
		default:
			$$->ilist = NULL;
			$$->blist = NULL;
		}
	}
	;

/*
Η δομή contents περιγράφει τα περιεχόμενα ενός box. Όπως είπαμε παραπάνω,
το box μπορεί να περιέχει άλλα subboxes εν είδει blist, ή item list εφόσον
πρόκειται για τελικό box. Δεδομένου ότι μια blist μπορεί να είναι κενή,
δίνεται η δυνατότητα να έχω κενό περιεχόμενο.
*/

contents
	: blist {
		$$ = contents_alloc($1, CONTENTS_BLIST);
	}

	| ilist {
		$$ = contents_alloc($1, CONTENTS_ILIST);
	}

	;

/*
Ακολουθεί ενότητα καθορισμού προσανοτολισμού box.
*/

orientation
	/*
	By default το box θεωρείται «οριζόντιο».
	*/

	: {
		$$ = 0;
	}

	/*
	Μπορούμε, ωστόσο, να ορίσουμε ρητά ότι το box είναι
	«οριζόντιο» με το keyword "hbox".
	*/

	| KWD_HBOX {
		$$ = 0;
	}

	/*
	Το box καθορίζεται «κάθετο» με το keyword "vbox".
	*/

	| KWD_VBOX {
		$$ = 1;
	}

	/*
	Το box καθορίζεται «οριζόντιο» σε όλο το πλάτος του πατρικού
	box (wide box) με το keyword "wbox".
	*/

	| KWD_WBOX {
		$$ = 2;
	}

	;

/*
Ακολουθεί ενότητα καθορισμού χρώματος box.
*/

bcolor
	/*
	Ο καθορισμός χρώματος είναι προαιρετικός
	*/

	: {
		$$ = NULL;
	}

	| color {
		$$ = $1;
	}

	;

/*
Τα boxes μπορούν να έχουν τίτλο.
*/

title
	/*
	Ο καθορισμός τίτλου είναι προαιρετικός.
	*/

	: {
		$$ = NULL;
	}

	/*
	Ο τίλος καθορίζεται μέσα σε αγκύλες, π.χ. [ "User Functions" ]
	*/

	| '[' STRING ']' {
		$$ = title_alloc($2, FONT_NORMAL);
	}

	/*
	Αν οι αγκύλες είναι διπλές, τότε ο τίτλος θα εκτυπωθεί με έντονα
	στοιχεία, π.χ. [[ "Kernel Space" ]]
	*/

	| LBRACE2 STRING RBRACE2 {
		$$ = title_alloc($2, FONT_BOLD);
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
	: '<' COLOR '>' {
		$$ = $2;
	}

	;

%%
