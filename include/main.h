#ifndef MAIN_H
#define MAIN_H

#include <stdlib.h>
#include <stdio.h>

static inline void debug_log(const char *msg) {
	fprintf(stderr, "[DEBUG] %s\n", msg);
}

#endif

