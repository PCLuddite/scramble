#include "finder.h"
#include "scramble.h"

/* 
 * checks to see if two letter counts are equal
 * expects params "alpha1" and "alpha2" to be an array with at least ALPHABET_SIZE elements
 */
static bool is_count_equal(const size_t alpha1[], const size_t alpha2[])
{
	size_t i = 0;
	for (; i < ALPHABET_SIZE; ++i) {
		if (alpha1[i] != alpha2[i]) {
			return false;
		}
	}
	return true;
}

/* 
 * finds the end of a word (a null-terminator or whitespace)
 */
static size_t find_end(const char* str)
{
	size_t len = 0;
	while(str[len] != '\0' && !isspace(str[len])) { /* scan the string until a null-terminator or space occours*/
		++len;
	}
	return len;
}

/* 
 * gets a letter count for a given string, returns string length
 * expects param "alpha" to be an array with at least ALPHABET_SIZE elements
 */
static size_t count_alpha(const char* str, size_t alpha[])
{
	size_t pos;
	memset(alpha, 0, ALPHABET_SIZE * sizeof*alpha);
	for(pos = 0; str[pos] != '\0'; ++pos) {
        	if (isalpha(str[pos])) {
			++alpha[tolower(str[pos]) - 'a'];
		}
	}
	return pos;
}


/*
 * checks wether param "alpha1" has the right letters to form "word"
 * param "alpha1" is expected to be at least ALPHABET_SIZE elements
 */
static bool contains_word(const size_t* alpha1, const char* word, size_t wordlen, bool anagrams_only) {
    size_t alpha2[ALPHABET_SIZE];
    size_t index;

    memset(alpha2, 0, ALPHABET_SIZE * sizeof*alpha2); /* initialize word count */

    for (index = 0; index < wordlen; ++index) { /* traverse the string and count the letters */
        if (isalpha(word[index])) { /* don't bother if not alpha */
            char c = tolower(word[index]);
            size_t letterindex = c - 'a'; /* get letter's position in the alphabet */
            if (alpha1[letterindex] == 0) {
                return false; /* the given letter set doesn't contain this letter */
            }
            else {
                ++alpha2[letterindex]; /* count the letter */
                if (alpha2[letterindex] > alpha1[letterindex]) {
                    return false; /* can't have more letters than the given letter set*/
                }
            }
        }
    }
    if (anagrams_only) { /* if good and only looking for anagrams, make sure counts equal */
        return is_count_equal(alpha1, alpha2);
    }
    else {
        return true; /* word matches criteria */
    }
}

/* 
 * scans the letter file "in" and appends matching words to the end of param "found"
 * param "found" will be modified when a word is found. It should be properly initialized.
 */
int findwords(const char* letters, cstring* found, bool anagrams_only, FILE* in)
{
	size_t alpha1[ALPHABET_SIZE]; /* letter count of the given letter set */
	char word[MAX_WORD]; /* the current word */
	int count = 0; /* the number of words found */

	size_t letters_len = count_alpha(letters, alpha1);

	while (fgets(word, MAX_WORD, in)) { /* read each word in the file */
		size_t wordlen = find_end(word); /* find the end of the word (skipping trailing whitespace) */
		if (wordlen <= letters_len && wordlen > 0 && /* if this word is longer, don't bother */
                    contains_word(alpha1, word, wordlen, anagrams_only)) { /* stores if this word meets the criteria */
			cstrcat_ln(found, word, wordlen); /* append to string */
			++count; /* increment count */
		}
	}
	found->ptr[found->size] = '\0'; /* add null-terminator */
	return count;
}
