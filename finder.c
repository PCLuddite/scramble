#include "finder.h"
#include "scramble.h"

/* checks to see if two letter counts are equal */
static bool is_count_equal(const size_t* alpha1, const size_t* alpha2)
{
	size_t i = 0;
	for (; i < ALPHABET_SIZE; ++i) {
		if (alpha1[i] != alpha2[i]) {
			return false;
		}
	}
	return true;
}

/* finds the end of a word (a null-terminator or whitespace) */
static size_t find_end(const char* str)
{
	size_t len = 0;
	while(str[len] != '\0' && !isspace(str[len])) { /* scan the string until a null-terminator or space occours*/
		++len;
	}
	return len;
}

/* gets a letter count for a given string, returns string length */
static size_t count_alpha(const char* str, size_t* alpha_count)
{
	size_t pos;
	memset(alpha_count, 0, ALPHABET_SIZE * sizeof*alpha_count);
    for(pos = 0; str[pos] != '\0'; ++pos) {
        if (isalpha(str[pos])) {
			alpha_count[tolower(str[pos]) - 'a']++;
		}
    }
    return pos;
}

static bool contains_word(const size_t* alpha1, const char* word, size_t wordlen, bool anagrams_only) {
    size_t alpha2[ALPHABET_SIZE];
    size_t index;

    memset(alpha2, 0, ALPHABET_SIZE * sizeof*alpha2);

    for (index = 0; index < wordlen; ++index) { /* traverse the string and count the letters */
        if (isalpha(word[index])) {
            char c = tolower(word[index]);
            size_t letterindex = c - 'a';
            if (alpha1[letterindex] == 0) { /* the given letter set doesn't contain this letter */
                return false;
            }
            else {
                ++alpha2[letterindex];
                if (alpha2[letterindex] > alpha1[letterindex]) { /* can't have more letters than the given letter set*/
                    return false;
                }
            }
        }
    }
    if (anagrams_only) { /* if good and only looking for anagrams, make sure they're equal */
        return is_count_equal(alpha1, alpha2);
    }
    else {
        return true;
    }
}

int findwords(const char* letters, cstring* found, bool anagrams_only, FILE* in)
{
	size_t alpha1[ALPHABET_SIZE]; /* letter count of the given letter set */
	int count = 0; /* the number of words found */
	char word[MAX_WORD]; /* the current word */

	size_t letters_len = count_alpha(letters, alpha1);

	while (fgets(word, MAX_WORD, in)) { /* read each word in the file */
		size_t wordlen = find_end(word); /* find the end of the word (skipping trailing whitespace) */
		if (wordlen <= letters_len && wordlen > 0) { /* if this word is longer, don't bother */
			bool good = contains_word(alpha1, word, wordlen, anagrams_only); /* stores if this word meets the criteria */
			if (good) {
				cstrcat_ln(found, word, wordlen); /* append to string */
				++count; /* increment count */
			}
		}
	}
	found->ptr[found->size] = '\0'; /* add null-terminator */
	return count;
}
