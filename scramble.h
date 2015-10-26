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

void cstrcat_ln(cstring* dest, const char* src, size_t len);

size_t GetWordPath(const char* arg0, char* buff, size_t buff_size);
void exit_error(const char* msg);
void show_usage(void);

void* emalloc(size_t siz);
void* erealloc(void* ptr, size_t newsize);

#endif
