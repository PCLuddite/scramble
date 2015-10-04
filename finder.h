#ifndef FINDER_H
#define FINDER_H

#include <string.h>
#include <ctype.h>

#include "scramble.h"

#if __STDC_VERSION__ >= 199901L
#include <stdbool.h>
#endif

#ifndef __bool_true_false_are_defined
typedef enum {
	false = 0, true
} bool;
#endif

/* reads a word file and finds words matching the given parameters, returns number of words found */
int findwords(const char* letters, cstring* found, bool anagrams_only, FILE* in);

#endif
