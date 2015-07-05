#include "scramble.h"

int main(int argc, char* argv[]) {
	char* letters;
	char path[MAX_PATH];
	FILE* wordsfile;
	STR_LIST found;
	size_t count;
	BOOL anagrams = FALSE;
	
	if (argc == 3) {
		if (strcmp(argv[1], "-a") == 0) { /* find anagrams only */
			letters = argv[2];
			anagrams = TRUE;
		}
		else {
			showUsage();
			return 1;
		}
	}
	else if (argc == 2) {
		letters = argv[1];
	}
	else {
		showUsage();
		return 1;
	}

	GetWordPath(argv[0], path, MAX_PATH);

	wordsfile = fopen(path, "r");

	if (wordsfile == NULL) {
		printf("unable to load %s", path);
		return 2;
	}

	found.list = malloc(MAX_WORD * sizeof*(found.list));
	if (found.list == NULL) {
		ERROR_NO_MEM;
	}
	
	found.maxsize = MAX_WORD;
	found.count = 0;

	if (found.list == NULL) {
		ERROR_NO_MEM;
	}
	count = findwords(letters, &found, anagrams, wordsfile);

	fclose(wordsfile);
	
	printf("%s", found.list);
	printf("%lu word(s) found.\n", count);

	free(found.list);

	return 0;
}

size_t findwords(const char* letters, STR_LIST* found, BOOL anagrams_only, FILE* in) {
	size_t alpha1[ALPHABET_SIZE]; /* letter count of the given letter set */
	size_t alpha2[ALPHABET_SIZE]; /* the letter count of the current word */
	size_t count = 0; /* the number of words found */
	char word[MAX_WORD]; /* the current word */

	size_t letters_len = count_alpha(letters, alpha1);

	while (fgets(word, MAX_WORD, in)) { /* read each word in the file */
		size_t len = find_end(word, strlen(word)); /* find the end of the word (skipping trailing whitespace) */
		if (len <= letters_len && len > 0) { /* if this word is longer, don't bother */
			BOOL good = TRUE; /* stores if this word meets the criteria */
			size_t i;
			memset(alpha2, 0, sizeof(size_t) * ALPHABET_SIZE);
			for (i = 0; good && i < len; ++i) { /* traverse the string and count the letters */
				if (isalpha(word[i])) {
					size_t index = tolower(word[i]) - 'a';
					if (alpha1[index] == 0) { /* the given letter set doesn't contain this letter */
						good = FALSE;
					}
					else {
						++alpha2[index];
						if (alpha2[index] > alpha1[index]) { /* can't have more letters than the given letter set*/
							good = FALSE;
						}
					}
				}
			}
			if (good && anagrams_only) { /* if good and only looking for anagrams, make sure they're equal */
				good = countcmp(alpha1, alpha2);
			}
			if (good) {
				list_append(found, word, len); /* append to list */
				++count; /* increment count */
			}
		}
	}
	found->list[found->count] = '\0'; /* add null-terminator */
	return count;
}

DWORD GetWordPath(const char* arg0, char* buff, size_t buff_size) {
	DWORD size;
	DWORD index;
	char* temp_path = malloc(buff_size);

#ifdef _WIN32
	size = GetModuleFileNameA(NULL, temp_path, buff_size);
#elif defined __linux__
	size = readlink("/proc/self/exe", temp_path, buff_size); /* hopefully it's a linux version that supports this */
#else
    strcpy(temp_path, arg0);
    size = strlen(temp_path);
#endif

    for(index = size - 1; index > 0; --index) { /* go backwards until we reach a directory separator */
        if (temp_path[index] == SEPARATOR[0]) {
            temp_path[index + 1] = '\0'; /* set where we want the string to end (one after the separator) */
            strcpy(buff, temp_path); /* copy the temp path to the buff */
            strcat(buff, "words.txt"); /* append the file we're looking for */
            free(temp_path);
            return index + 9;
        }
    }
    
    strcpy(buff, ".");
    strcat(buff, SEPARATOR);
    strcat(buff, "words.txt"); /* fail safe, hopefully it's in the startup path */
    
    free(temp_path);
    return 11; /* yeah, magic numbers, I know. It's the string length. */
}

void showError(const char* msg) {
	printf("error: %s\n", msg);
	exit(2);
}

size_t count_alpha(const char* str, size_t* alpha_count) {
	size_t pos;
	memset(alpha_count, 0, ALPHABET_SIZE * sizeof*alpha_count);
    	for(pos = 0; str[pos] != '\0'; ++pos) {
        	if (isalpha(str[pos])) {
			alpha_count[tolower(str[pos]) - 'a']++;
		}
    	}
    	return pos;
}

BOOL countcmp(size_t* alpha1, size_t* alpha2) {
    	size_t i = 0;
	for (; i < ALPHABET_SIZE; ++i) {
		if (alpha1[i] != alpha2[i]) {
            		return FALSE;
        	}
	}
    	return TRUE;
}

void list_append(STR_LIST* found, const char* word, size_t len) {
	size_t pos = found->count;
	found->count += len+1; /* also include space for newline char */
	if (found->count >= found->maxsize) { /* max-size has been reached, time to realloc */
		char* new_words;
		do {
			found->maxsize *= 2; /* double list size until we have space */
		} while(found->count >= found->maxsize);
		new_words = realloc(found->list, found->maxsize * sizeof(*new_words));
		if (new_words == NULL) {
			ERROR_NO_MEM; /* yikes */
		}
        found->list = new_words;
    }
    memcpy(found->list + pos, word, (len+1) * sizeof*word); /* copy the word to the end of the string */
    found->list[found->count++] = '\n'; /* add newline char */
}

size_t find_end(const char* str, size_t len) {
	while (len > 0 && isspace(str[len - 1])) {
		--len;
	}
	return len;
}

void showUsage(void) {
    printf("usage: scramble [-a] <word>\n");
    printf("\n-a\tfind only anagrams\n");
}
