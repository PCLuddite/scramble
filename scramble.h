#ifndef _SCRAMBE_H
#define _SCRAMBE_H

/* main header files */
#include <stdlib.h> /* the universe */
#include <stdio.h>
#include <string.h>

#ifdef __linux__
#include <unistd.h> /* for readlink() */
#endif /* __linux__ */

#if defined _WIN32 || _MSDOS
#define SEPARATOR '\\'
#else /* assume UNIX-like system */
#define SEPARATOR '/'
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
#define INTIAL_SIZE 5 /* initial found list size */
#define ALPHABET_SIZE 26 /* as if this would need to be changed */

typedef struct {
	char** list;
	size_t count;
	size_t maxsize;
} STR_LIST; /* simple struct for storing pointers to strings */

void list_append(STR_LIST* found, char* word, size_t pos); /* appends a pointer to a string in a string list */
size_t count_alpha(const char* str, size_t* alpha_count); /* gets a letter count for a given string, returns string length */
BOOL countcmp(size_t* alpha1, size_t* alpha2); /* checks to see if two letter counts are equal */
size_t find_end(const char* str, size_t len); /* finds the end of a word (basically trimend) */

DWORD GetWordPath(const char* arg0, char* buff, DWORD buff_size); /* gets the path of a word file */
void showError(const char* msg); /* shows an error message and exits */
void showUsage(void); /* shows the program usage */

void findwords(const char* letters, STR_LIST* found, BOOL anagrams_only, FILE* in); /* reads a word file and finds words matching the given parameters */

#define ERROR_NO_MEM showError("memory allocation failed") /* no memory error */

#endif
