#include "boxes.h"
#include <unistd.h>
#include <string.h>
#include <ctype.h>

void main(int argc, char *argv[]) {
	int c;
	int errs = 0;

	if (progname = strrchr(argv[0], '/'))
	progname++;

	else
	progname = argv[0];

	while ((c = getopt(argc, argv, ":th")) != EOF) {
		switch (c) {
		case 't':
			printer = printer_text;
			break;
		case 'h':
			printer = printer_html;
			break;
		case '?':
			fprintf(stderr, "%s: %c: invalid option\n", progname,
				isprint(optopt) ? optopt : '?');
			errs++;
			break;
		}
	}

	if (errs)
	exit(EXIT_USAGE);

	argc -= optind;
	argv += optind;

	if (argc)
	for (; argc > 0; argc--, argv++) {
		source = *argv;
		lineno = 1;

		if (freopen(source, "r", stdin)) {
			yyparse();
			continue;
		}

		fprintf(stderr, "%s: ", progname);
		perror(source);
		errcnt++;
	}

	else {
		lineno = 1;
		yyparse();
	}

	if (errcnt) {
		fprintf(stderr, "%s: %ld errors encounterd\n", progname, errcnt);
		exit(EXIT_ERROR);
	}

	exit(EXIT_OK);
}
