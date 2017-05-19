#include <stdlib.h>
#include <stdio.h>

#define EXIT_OK 0
#define EXIT_USAGE 1
#define EXIT_ERROR 2
#define EXIT_MEMORY 3

#define BOX_UNDEFINED 0
#define BOX_HORIZONTAL 1
#define BOX_VERTICAL 2
#define BOX_WIDE 3

#define FONT_UNDEFINED 0
#define FONT_NORMAL 1
#define FONT_BOLD 1

// Όλα είναι boxes. Κάθε box μπορεί να έχει επικεφαλίδα, να περιλαμβάνει άλλα
// subboxes, ή να είναι «τελικό» οπότε απλώς περιλαμβάνει κάποια items.
//
// Τα boxes μπορούν να είναι οριζόντια (default) ή κάθετα, όπου η καθετότητα
// ορίζει να εκτυπώνεται η επικεφαλίδα από κάτω προς τα πάνω και τα τυχόν
// items οριζόντια, το ένα κάτω από το άλλο κανονικά. Αντίθετα, ένα οριζόντιο
// box σημαίνει οριζόντια εκτύπωση επικεφαλίδας και τα items εκτυπωμένα
// κάθετα, από κάτω προς τα πάνω.
//
// Τα boxes εκτυπώνονται το ένα δίπλα στο άλλο, εκτός και αν πρόκειται για box
// που ορίζει νέα ενότητα (section) για το επόμενο box, οπότε κάνουμε break.

typedef struct {
	char *title;
	unsigned int font:2;		// 0: undefined, 1: normal, 2: bold
	unsigned int type:2;		// 0: undefined, 1: horizontal, 2: vertical, 3: wide
	char *color;			// box background color
	unsigned int section:1;		// next box position (0: inline, 1: break)
	struct BLIST *blist;		// subbox list
	struct ILIST *ilist;		// item list
} BOX;

typedef struct {
	unsigned int type:2;
	char *color;
	char *title;
	unsigned int font;
} BATTRS;

// Οπωσδήποτε θα χρειαστούμε λίστες από boxes. Το ίδιο το input είναι μια
// box list, ενώ τα ίδια τα boxes μπορούν να περιέχεουν άλλα boxes ως box
// list.

typedef struct BLIST {
	BOX *box;
	struct BLIST *nxt;
} BLIST;

// Τα boxes μπορούν αν είναι «τελικά», τουτέστιν boxes που δεν περιέχουν
// subboxes, αλλά items. Τα items περιλαμβάνουν το λεκτικό τους και το
// χρώμα εκτύπωσης.

typedef struct {
	char *s;
	char *color;
} ITEM;

// Είναι σίγουρο ότι θα χρειαστούμε item lists. Πράγματι τα «τελικά» boxes
// δεν περιλαμβάνουν subboxes αλλά περιλαμβάνουν items τα οποία σχηματίζουν
// μια item list.

typedef struct ILIST {
	ITEM *item;
	struct ILIST *nxt;
} ILIST;

extern char *progname;
extern char *source;
extern size_t lineno;
extern size_t errcnt;

extern BLIST *root;
extern void (*printer)(BLIST *, size_t);

extern int yyparse();
extern int yylex();
extern int yyerror(char *);

extern void main(int, char *[]);
extern void error(char *);
extern void fatal(char *, const unsigned int);

extern BOX *box_alloc(BATTRS *);
extern BLIST *blist_alloc(BOX *);
extern BLIST *box_push(BLIST *, BOX *);
extern BLIST *break_push(BLIST *);
extern BLIST *blist_push(BLIST *);

extern ITEM *item_alloc(char *, char *);
extern ILIST *ilist_alloc(ITEM *);
extern ILIST *item_push(ILIST *, ITEM *);

extern char *strsave(char *);
extern char *strnsave(char *, size_t);

extern void printer_text(BLIST *, size_t);
extern void printer_html(BLIST *, size_t);
