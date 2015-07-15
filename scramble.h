#ifndef _SCRAMBE_H
#define _SCRAMBE_H

/* main header files */
#include <stdlib.h> /* the universe */
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#ifdef __linux__
#include <unistd.h> /* for readlink() */
#endif /* __linux__ */

#if defined _WIN32 || _MSDOS
#define SEPARATOR "\\"
#else /* assume UNIX-like system */
#define SEPARATOR "/"
#endif /* _WIN32 || _MSDOS */

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h> /* for GetModuleFileNameA */
#endif /* _WIN32 */

#ifndef BOOL
#define BOOL int
#endif /* BOOL */

#ifndef TRUE
#define TRUE 1
#endif /* TRUE */

#ifndef FALSE
#define FALSE 0
#endif /* FALSE */

#ifndef DWORD
#define DWORD unsigned long
#endif /* DWORD */

#ifndef MAX_PATH
#define MAX_PATH 256 /* todo: determine actual MAX_PATH on Linux 6/9/15 */
#endif /* MAX_PATH */

#define MAX_WORD 100 /* max buffer for word storage */
#define ALPHABET_SIZE 26 /* as if this would need to be changed */

typedef struct {
	char* ptr;
	size_t size;
	size_t maxsize;
} CSTRING; /* simple struct for storing a pointer to a string */

void cstr_catln(CSTRING* dest, const char* src, size_t len); /* appends the src string and a new line '\n' to dest string */
size_t count_alpha(const char* str, size_t* alpha_count); /* gets a letter count for a given string, returns string length */
BOOL countcmp(size_t* alpha1, size_t* alpha2); /* checks to see if two letter counts are equal */
size_t find_end(const char* str); /* finds the end of a word (a null-terminator or whitespace) */

DWORD GetWordPath(const char* arg0, char* buff, size_t buff_size); /* gets the path of a word file */
void showError(const char* msg); /* shows an error message and exits */
void showUsage(void); /* shows the program usage */

/* reads a word file and finds words matching the given parameters, returns number of words found */
int findwords(const char* letters, CSTRING* found, BOOL anagrams_only, FILE* in);

#define ERROR_NO_MEM showError("memory allocation failed") /* no memory error */

#endif
