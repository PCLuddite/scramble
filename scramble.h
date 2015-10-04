#ifndef _SCRAMBE_H
#define _SCRAMBE_H

/* main header files */
#include <stdlib.h>
#include <stdio.h>

#ifdef __linux__
#include <unistd.h> /* for readlink() */
#endif /* __unix__ */

#if defined _WIN32 || _MSDOS
#define SEPARATOR "\\"
#else /* assume UNIX-like system */
#define SEPARATOR "/"
#endif /* _WIN32 || _MSDOS */

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h> /* for GetModuleFileNameA */
#endif /* _WIN32 */

#ifndef MAX_PATH
#define MAX_PATH 256 /* TODO: determine actual MAX_PATH on Linux 6/9/15 */
#endif /* MAX_PATH */

#define MAX_WORD 100 /* max buffer for word storage */
#define ALPHABET_SIZE 26 /* as if this would need to be changed */

typedef struct {
	char* ptr;
	size_t size;
	size_t maxsize;
} cstring; /* simple struct for storing a pointer to a string */

void cstrcat_ln(cstring* dest, const char* src, size_t len); /* appends the src string and a new line '\n' to dest string */

size_t GetWordPath(const char* arg0, char* buff, size_t buff_size); /* gets the path of a word file */
void showError(const char* msg); /* shows an error message and exits */
void showUsage(void); /* shows the program usage */

void* emalloc(size_t siz); /* calls malloc and exits program if failed */
void* erealloc(void* ptr, size_t newsize); /* calls realloc and exits program if failed */

#endif
