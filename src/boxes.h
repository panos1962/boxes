#include <stdlib.h>
#include <stdio.h>

#define EXIT_OK 0
#define EXIT_USAGE 1
#define EXIT_ERROR 2
#define EXIT_MEMORY 3

#define BOX_HORIZONTAL 0
#define BOX_VERTICAL 1
#define BOX_WIDE 2

#define FONT_NORMAL 0
#define FONT_BOLD 1
#define FONT_ITALIC 2
#define FONT_UNDERLINE 4

#define CONTENTS_BLIST 0
#define CONTENTS_ILIST 1

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
	unsigned int font:3;		// 0: normal, 1: bold, 2: italic, 4: underline
	unsigned int vertical:2;	// 0: horizontal, 1: vertical, 2: wide
	char *color;			// box background color
	unsigned int section:1;		// next box position (0: inline, 1: break)
	struct BLIST *blist;		// subbox list
	struct ILIST *ilist;		// item list
} BOX;

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

// Για τις ανάγκες της ανάγνωσης και αναγνώρισης του input χρειάζεται να
// δημιουργήσουμε δομή τίτλου στην οποία περιλαμβάνεται το λεκτικό του
// τίτλου και τα χαρακτηριστικά γραφής. Η δομή τίτλου χρησιμοποιείται
// μόνο προσωρινά, καθώς τα στοιχεία της δομής μεταγράφονται αυτούσια
// στο αντίστοιχο box.

typedef struct {
	char *s;
	unsigned int font;
} TITLE;

typedef struct {
	union {
		BLIST *b;
		ILIST *i;
	} list;
	unsigned short type:1;
} CONTENTS;

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

extern BOX *box_alloc(void);
extern BLIST *blist_alloc(BOX *);
extern BLIST *box_push(BLIST *, BOX *);
extern BLIST *break_push(BLIST *);
extern BLIST *blist_push(BLIST *);

extern ITEM *item_alloc(char *, char *);
extern ILIST *ilist_alloc(ITEM *);
extern ILIST *item_push(ILIST *, ITEM *);

extern TITLE *title_alloc(char *, unsigned int);
extern CONTENTS *contents_alloc(void *, unsigned int);

extern char *strsave(char *);
extern char *strnsave(char *, size_t);

extern void printer_text(BLIST *, size_t);
extern void printer_html(BLIST *, size_t);
