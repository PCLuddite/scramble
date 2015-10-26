#include "scramble.h"
#include "finder.h"

/*
 * displays program usage
 */
void show_usage(void)
{
    fputs("usage: scramble [--anagrams|-a] <word>\n", stdout);
    fputs("\n--anagrams,-a\tfind only anagrams\n", stdout);
}

/*
 * program main
 */
int main(int argc, char* argv[])
{
    char* letters;
    char path[MAX_PATH];
    FILE* wordsfile;
    cstring found;
    int count;
    bool anagrams = false;

    if (argc == 3) {
        if (strcmp(argv[1], "-a") == 0 || strcmp(argv[1], "--anagrams") == 0) { /* find anagrams only */
            letters = argv[2];
            anagrams = true;
        }
        else {
            show_usage();
            return EXIT_FAILURE;
        }
    }
    else if (argc == 2) {
        letters = argv[1];
    }
    else {
        show_usage();
        return EXIT_FAILURE;
    }

    GetWordPath(argv[0], path, MAX_PATH);

    wordsfile = fopen(path, "r");

    if (wordsfile == NULL) {
        char format[] = "unable to load \"%s\"";
        char error_msg[MAX_PATH + sizeof format];
        sprintf(error_msg, format, path);
        exit_error(error_msg);
    }

    /* initialize cstring */
    found.ptr = emalloc(MAX_WORD * sizeof*found.ptr);
    found.maxsize = MAX_WORD;
    found.size = 0;

    count = findwords(letters, &found, anagrams, wordsfile);

    fclose(wordsfile);

    fputs(found.ptr, stdout);
    printf("%i word(s) found.\n", count);

    free(found.ptr);

    return EXIT_SUCCESS;
}

/*
 * Appends src and a newline ('\n') to dest
 * src is expected to be at least len characters
 */
void cstrcat_ln(cstring* dest, const char* src, size_t len)
{
    size_t pos = dest->size; /* store the old length of the dest */
    dest->size += len; /* calculate the new length of the dest */
    if (dest->size + 1 >= dest->maxsize) { /* max-size has been reached, time to realloc */
        char* new_words;
        do {
            dest->maxsize *= 2; /* double dest size until we have space */
        } while(dest->size + 1 >= dest->maxsize);

        dest->ptr = erealloc(dest->ptr, dest->maxsize * sizeof*new_words);
    }
    memcpy(dest->ptr + pos, src, len * sizeof*src); /* copy the src to the end of the string */
    dest->ptr[dest->size++] = '\n'; /* add newline char and increment size by 1 */
}

/*
 * Prints an error, then terminates the program with EXIT_FAILURE
 */
void exit_error(const char* msg)
{
    printf("error: %s\n", msg);
    exit(EXIT_FAILURE);
}

/*
 * malloc, but terminates program on error
 */
void* emalloc(size_t size)
{
    void* newptr = malloc(size);
    if (newptr == NULL) {
        exit_error("call to malloc failed");
    }
    return newptr;
}

/*
 * realloc, but terminates program on error
 */
void* erealloc(void* ptr, size_t newsize)
{
    void* newptr = realloc(ptr, newsize);
    if (newptr == NULL) {
       exit_error("call to realloc failed");
    }
    return newptr;
}

/*
 * Sets buff to the path of the word file
 * argv[0] is passed as arg0, but it may not be used depending on the implementation
 */
size_t GetWordPath(const char* arg0, char* buff, size_t buff_size)
{
    const char word_file[] = "words.txt";
    char* temp_path = emalloc(buff_size);
#ifdef _WIN32
    /* if windows, define things in terms of DWORD */
    DWORD size;
    DWORD index;
    size = GetModuleFileNameA(NULL, temp_path, buff_size);
#elif defined __linux__
    /* POSIX likes ssize_t */
    ssize_t size;
    ssize_t index;
    size = readlink("/proc/self/exe", temp_path, buff_size); /* hopefully it's a linux version that supports this */
#else
    /* just hope arg0 contains something useful */
    size_t size;
    size_t index;
    strcpy(temp_path, arg0); /* hope that the path is in arg0 */
    size = strlen(temp_path);
#endif

    for(index = size - 1; index > 0; --index) { /* go backwards until we reach a directory separator */
        if (temp_path[index] == SEPARATOR[0]) {
            temp_path[index + 1] = '\0'; /* set where we want the string to end (one after the separator) */

            if (index + sizeof word_file + 1 >= buff_size)
                goto overflow;

            sprintf(buff, "%s%s", temp_path, word_file); /* copy the temp path and file name to the buffer */
            goto cleanup;
        }
    }

    if (sizeof word_file + 3 >= buff_size)
        goto overflow;
    
    sprintf(buff, ".%c%s", SEPARATOR[0], word_file); /* fail safe, hopefully it's in the startup path */

    goto cleanup;

overflow:
    exit_error("buffer overflow when building word file path");
cleanup:
    free(temp_path);
    return strlen(buff);
}
