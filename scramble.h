#ifndef _SCRAMBE_H
#define _SCRAMBE_H

/*#define _CRT_SECURE_NO_WARNINGS*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifdef __linux__
#include <unistd.h>
#endif

#if defined _WIN32 || _MSDOS
#define SEPARATOR '\\'
#else
#define SEPARATOR '/'
#endif

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
/*#define strcmp _strcmpi*/
#endif

#ifndef BOOL
#define BOOL int
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef DWORD
#define DWORD unsigned int
#endif

#ifndef MAX_PATH
#define MAX_PATH 256
#endif

#define MAX_WORD 100
#define INTIAL_SIZE 5
#define ALPHABET_SIZE 26 /* as if this would need to be changed */

typedef struct {
	char** list;
	size_t count;
	size_t maxsize;
} STR_LIST;


DWORD GetWordPath(const char* arg0, char* buff, DWORD buff_size);
void getCount(const char* str, size_t len, size_t* alpha_count);
void showError(const char* msg);
BOOL countcmp(size_t* alpha1, size_t* alpha2);
size_t find_end(const char* str, size_t len);
void insert(STR_LIST* found, char* word, size_t pos);
void findwords(const char* letters, size_t letters_len, STR_LIST* found, BOOL anagrams_only, FILE* in);
void showUsage(void);

#define ERROR_NO_MEM showError("memory allocation failed")

#endif
