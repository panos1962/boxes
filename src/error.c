#include "boxes.h"

void error(char *s) {
	if (progname)
	fprintf(stderr, "%s: ", progname);

	if (source)
	fprintf(stderr, "%s: ", source);

	if (lineno)
	fprintf(stderr, "line %ld: ", lineno);

	if (!s)
	return;

	fputs(s, stderr);
	putc('\n', stderr);
}

void fatal(char *s, const unsigned int err) {
	error(s);
	exit(err);
}

int yyerror(char *s) {
	error(s);
	errcnt++;
	return 0;
}
