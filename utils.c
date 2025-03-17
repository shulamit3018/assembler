#include <string.h>
#include <ctype.h>
#include "utils.h"

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

FILE * open_file (char *filename, char *format) {
	FILE *f;
	
	f = fopen(filename, format);
	if (!f)
	{
		printf("File %s does not exist.\n", filename);
		return NULL;
	}
	return f;
}

void get_filename(char *base, char *extension, char *filename) {
    strcpy(filename, "testfiles\\");
	strcat(filename, base);
	strcat(filename, ".");
	strcat(filename, extension);
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