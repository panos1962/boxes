#include "boxes.h"
#include <unistd.h>
#include <string.h>
#include <ctype.h>

void main(int argc, char *argv[]) {
	int c;
	int errs = 0;

	// Αφαιρούμε το path που ενδεχομένως ενυπάρχει στο όνομα του
	// προγράμματος.

	if (progname = strrchr(argv[0], '/'))
	progname++;

	else
	progname = argv[0];

	// Διαχειριζόμαστε τυχόν options που έχουν δοθεί στο command line.

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

	// Ξεσκαρτάρουμε τις options που έχουν δοθεί στο command line, οπότε
	// τα υπόλοιπα command line arguments πρέπει να είναι input files.

	argc -= optind;
	argv += optind;

	// Εφόσον έχουν καθοριστεί input files στο command line τα διαβάζουμε
	// ένα προς ένα ως ένα ενιαίο input.

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

	// Αν δεν καθορίστηκαν input files στο command line, τότε διαβάζουμε
	// από το standard input.

	else {
		lineno = 1;
		yyparse();
	}

	if (errcnt) {
		fprintf(stderr, "%s: %ld errors encounterd\n", progname, errcnt);
		exit(EXIT_ERROR);
	}

	if (printer)
	printer(root, 0);

	exit(EXIT_OK);
}
