%{

#include "boxes.h"
#include "x.tab.h"

static int kwd_lookup(char *);

%}

%%

(#|\/\/).*$ {
	// Οτιδήποτε ακολουθεί μετά από "#", ή "//" θεωρείται σχόλιο και αγνοείται.
}

[ \t] {
	// Λευκοί χαρακτήρες αγνοούνται.
}

\"([^\\\"]|\\.)*\" {
	// Οτιδήποτε βρίσκεται ανάμεσα σε διπλά quotes θεωρείται string.

	char *p, *q;

	if (!(p = malloc(yyleng - 1)))
	fatal("flex: string: out of memory", EXIT_MEMORY);

	for (yylval.s = p, q = yytext + 1; *q; q++) {
		if (*q != '\\') {
			*p++ = *q;
			continue;
		}

		switch (*(++q)) {
		case '\\':
			*p++ = '\\';
			break;
		case 'n':
			*p++ = '\n';
			break;
		case 't':
			*p++ = '\t';
			break;
		case '"':
			*p++ = '"';
			break;
		default:
			*p++ = '\\';
			*p++ = *q;
			break;
		}
	}

	*(--p) = '\0';
	return STRING;
}

\n {
	lineno++;
}

[a-zA-Z0-9_]* {
	int kwd;
	char *p;

	if (kwd = kwd_lookup(yytext))
	return kwd;

	if (!(p = malloc(yyleng - 1)))
	fatal("flex: string: out of memory", EXIT_MEMORY);

	yylval.s = strnsave(yytext, yyleng);
	return COLOR;
}

"[" {
	return LBRACE1;
}

"]" {
	return RBRACE1;
}

"[[" {
	return LBRACE2;
}

"]]" {
	return RBRACE2;
}

"..." {
	yylval.s = strnsave(yytext, yyleng);
	return STRING;
}

. {
	return *yytext;
}

%%

static struct {
	char *keyword;
	int token;
} kwd_table[] = {
	"hbox",		KWD_HBOX,
	"vbox",		KWD_VBOX,
};

static int kwd_lookup(char *s) {
	int i;

	for (i = 0; i < sizeof(kwd_table) / sizeof(*kwd_table); i++) {
		if (!strcmp(s, kwd_table[i].keyword))
		return kwd_table[i].token;
	}

	return 0;
}
