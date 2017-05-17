#include "boxes.h"

char *progname = "boxes";
char *source = NULL;
size_t lineno = 0;
size_t errcnt = 0;

BLIST *root = NULL;
void (*printer)(BLIST *, size_t) = NULL;
