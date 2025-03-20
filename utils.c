#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "utils.h"

#ifdef _WIN32
    #define PATH_SEPARATOR '\\'
#else
    #define PATH_SEPARATOR '/'
#endif

#define TEST_FILES_PATH "testfiles"

int is_whitespaces(char *word) {
    while (*word && isspace(*word)) word++;
    return (*word == '\0' || *word == '\n');
}

ErrorCode get_word(char *line, char *word, char ** next_word, int is_last) {
    char *pos = line;

    /* check for an unexpected comma */
    if (*line == ',') {
        return ERR_COMMA_EXTRA;
    }

    /* Skip leading spaces */
    while (*pos && isspace(*pos)) pos++;

    /* Get a word */
    while (*pos && !isspace(*pos) && *pos != ',') {
        *word = *pos;
        word++;
        pos++;
    }
    *word = '\0';
    
    /* Skip trailing spaces */
    while (*pos && isspace(*pos)) pos++;
    if (next_word) {
        *next_word = pos;
    }
    /* If this is the last word, check for trailing text */
    if (is_last == LAST_WORD && *pos) {
        return ERR_TRAILING_TEXT;
    }

    return SUCCESS;
}

ErrorCode get_comma(char *line, char ** next_word) {
    char *pos = line;

    /* Skip leading spaces */
    while (*pos && isspace(*pos)) pos++;

    /* Get a comma */
    if (*pos != ',') {
        return ERR_COMMA_MISSING;
    }
    pos++;
    
    /* Skip trailing spaces */
    while (*pos && isspace(*pos)) pos++;
    if (next_word) {
        *next_word = pos;
    }

    return SUCCESS;
}

ErrorCode get_number(char *word, int *value) {
    char *endptr;
    long result = strtol(word, &endptr, 10);

    if (endptr == word && *endptr != '\0') {
		return ERR_NUMBER_ILLEGAL;
    }

	/* Ensure the number is within the acceptable range for the assembler */
    if ((result >= -(1 << 20)) && (result < (1 << 20))) {
		*value = (int)result;
		return SUCCESS;
	}
	else {
		return ERR_NUMBER_OUT_OF_RANGE;
	}

}

int is_filename_too_long(char *base) {
    /* add 1 character for path separator, 3 for extension, 1 for dot */
    if ((strlen(TEST_FILES_PATH) + strlen(base) + 5) >= MAX_FILE_NAME) {
        return 1;
    }
    return 0;
}

void get_filename(char *base, char *extension, char *filename) {
    sprintf(filename,"%s%c%s.%s", TEST_FILES_PATH, PATH_SEPARATOR, base, extension);
}

int is_line_too_long(FILE *file, char *line) {
    /* Line should contain '\n' */
    if (!strchr(line, '\n')) {
        /* Check if end of file */
        char c = fgetc(file);
        if (c == EOF) {
            return 0;
        }

        /* Read till '\n' or end of file */
        do {
            c = fgetc(file);
        } while (c != EOF && c != '\n');
        return 1;
    }
    return 0;
}

char * my_strdup(char *string) {
    char *copy;
    if (string == NULL) {
        return NULL;
    }
    copy = malloc(strlen(string) + 1);
    if (copy) {
        strcpy(copy, string);
    }
    return copy;
}
