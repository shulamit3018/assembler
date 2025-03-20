#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "utils.h"
#include "language.h"
#include "macro.h"

int macro_process(char *filename, FILE *source, FILE *destination)
{
	char name[LINE_LEN];
	int in_macro = 0; /* Flag indicating whether inside a macro definition */
	int line_number = 0;

	char line[LINE_LEN];
	char first_word[LINE_LEN];
	char *rest_of_line;
	ErrorCode error;
	int error_state = 0; /* Tracks if an error has occurred */
	
	/* Read file line by line */
	while(fgets(line, LINE_LEN, source))
	{
		line_number++;
		
		/* Ensure null termination */
		line[LINE_LEN-1] = '\0';
		rest_of_line = line;

		/* Check if the line exceeds the allowed length */
		if (is_line_too_long(source, line)) {
			is_error(ERR_LINE_TOO_LONG, &error_state, filename, line_number, NULL);
			continue;
		}

		 /* Extract the first word of the line */
		error = get_word(rest_of_line, first_word, &rest_of_line, LAST_WORD_DONT_CARE);
		if (is_error(error, &error_state, filename, line_number, NULL)) {
			continue;
		}
		
		if (!in_macro) {
			/* Check if this is the beginning of a macro definition */
			if(strcmp(first_word, "mcro") == 0) {
				error = get_word(rest_of_line, name, NULL, LAST_WORD);
				if (is_error(error, &error_state, filename, line_number, NULL)) {
					continue;
				}
				/* Ensure the macro name is not empty */
				if (strlen(name) == 0) {
					is_error(ERR_MACRO_MISSING_NAME, &error_state, filename, line_number, NULL);
					continue;
				}
				
				/* Add macro to the table */
				error = add_macro(name);
				if (is_error(error, &error_state, filename, line_number, NULL)) {
					continue;
				}
		
				/* Entering macro definition mode */
				in_macro = 1;
			}
			else {
				/* If this is a macro name - replace with content */
				char *content;
				
				if (get_macro(first_word, &content) == SUCCESS) {
					fputs(content, destination);
					/* Ensure there is no trailing text after a macro call */
					if (!is_whitespaces(rest_of_line)) {
						is_error(ERR_TRAILING_TEXT, &error_state, filename, line_number, NULL);
						continue;
					}
				}
				/* Otherwise, keep the original line */
				else {
					fputs(line, destination);
				}
			}
		}
		/* Inside a macro definition */
		else {
			/* Check for the end marker */
			if(strcmp(first_word, "mcroend") == 0)
			{
				/* Exit macro definition mode */
				in_macro = 0;
				if (!is_whitespaces(rest_of_line)) {
					is_error(ERR_TRAILING_TEXT, &error_state, filename, line_number, NULL);
					continue;
				}
			}
			else {
				/* Append line to the macro's content */
				add_macro_content(name, line);
			}
		}
	}

	return error_state;
}

/* Macro table --------------------------------------------- */

# define MACRO_NAME_LEN 31

/* Struct for macro definition */
typedef struct macro_t {
    char *name;
    char *content;
    struct macro_t *next;
} macro_t;

/* Head of linked list of macro definitions*/
static macro_t *macro_list = NULL;

ErrorCode add_macro(char *name) {
	int i;
	macro_t *new_macro;

	/* Validate macro name */
    /* Check if the macro name is a reserved word */
    if (is_reserved_word(name)) {
        return ERR_MACRO_RESERVED;
    }
    
	/* Ensure the macro name is within the allowed length */
    if (strlen(name) > MACRO_NAME_LEN) {
        return ERR_MACRO_NAME_TOO_LONG;
    }

	/* Ensure that the macro name contains only letters or underscores */
	for (i = 0; i < strlen(name); i++) {
		if (!isalpha(name[i]) && name[i] != '_') {
			return ERR_MACRO_ILLEGAL_NAME;
		}
	}
    
    /* Check if macro already exists */
	if (get_macro(name, NULL) == SUCCESS) {
        return ERR_MACRO_REDEFINITION;
    }
    
    /* Allocate and insert new macro */
    new_macro = (macro_t *)malloc(sizeof(macro_t));
    if (!new_macro) {
        return ERR_OUT_OF_MEMORY;
    }

    new_macro->name = my_strdup(name);
	new_macro->content = NULL;
    new_macro->next = macro_list;
    macro_list = new_macro;
    
    return SUCCESS;
}

ErrorCode add_macro_content(char *name, char *content) {
    macro_t *current = macro_list;
	while (current) {
        if (strcmp(current->name, name) == 0) {
			/* Append content to the existing macro */
			if (current->content == NULL) {
				current->content = my_strdup(content);
			}
			else {
				current->content = (char *)realloc(current->content, strlen(current->content) + strlen(content) + 1);
				strcat(current->content, content);
			}
            return SUCCESS;
        }
        current = current->next;
    }

	/* Should never happen */
	return ERR_INTERNAL_ASSERT;
}

int is_macro(char *name) {
	return get_macro(name, NULL) == SUCCESS;
}

ErrorCode get_macro(char *name, char **content) {
    macro_t *current = macro_list;
	/* Search for the macro in the macro list */
    while (current) {
        if (strcmp(current->name, name) == 0) {
			if (content) {
            	*content = current->content;
			}
            return SUCCESS;
        }
        current = current->next;
    }
    return ERR_INTERNAL_ASSERT;
}

void purge_macros() {
    macro_t *current = macro_list;
	/* Free the macro table*/
    while (current) {
        macro_t *next = current->next;
        free(current->name);
        free(current->content);
        free(current);
        current = next;
    }
    macro_list = NULL;
}
