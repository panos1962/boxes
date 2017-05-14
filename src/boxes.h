#include <stdlib.h>
#include <stdio.h>

#define EXIT_OK 0
#define EXIT_USAGE 1
#define EXIT_ERROR 2
#define EXIT_MEMORY 3

#define FONT_BOLD 1
#define FONT_ITALIC 2
#define FONT_UNDERLINE 4

typedef struct {
	unsigned int section:1;

	char *title;
	unsigned int font:3;		// 0: normal, 1: bold, 2: italic, 4: underline

	unsigned int vertical:1;	// 0: horizontal, 1: vertical
	char *color;			// box background color

	struct BLIST *blist;
	struct ILIST *ilist;
} BOX;

typedef struct BLIST {
	BOX *box;
	struct BLIST *nxt;
} BLIST;

typedef struct {
	char *s;
	char *color;
} ITEM;

typedef struct ILIST {
	ITEM *item;
	struct ILIST *nxt;
} ILIST;

typedef struct {
	char *s;
	unsigned int font;
} TITLE;

extern char *progname;
extern char *source;
extern size_t lineno;
extern size_t errcnt;

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

extern ITEM *item_alloc(char *, char *);
extern ILIST *ilist_alloc(ITEM *);
extern ILIST *item_push(ILIST *, ITEM *);

extern TITLE *title_alloc(char *, unsigned int);

extern char *strsave(char *);
extern char *strnsave(char *, size_t);

extern void printer_text(BLIST *, size_t);
extern void printer_html(BLIST *, size_t);
