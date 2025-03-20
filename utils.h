#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include "error_codes.h"

#define LINE_LEN 81 /* including zero termination */
#define MAX_FILE_NAME 100

/* Indicates whether this should be the last word in line */
typedef enum {
    LAST_WORD_DONT_CARE = 0,
    LAST_WORD = 1
} last_word_t;

/**
Checks if the line contains only whitespace characters.  
   @param line: The input string.  
   @return 1 if the line is all whitespace, 0 otherwise.
*/
int is_whitespaces(char *line);

/**
Extracts the next word
   @param line: The input string.  
   @param word: Buffer to store the extracted word.  
   @param next_word: Pointer to the rest of the line after the word.  
   @param is_last_word: Indicates if this is the last word.  
   @return SUCCESS if a word found, error otherwise.
*/
ErrorCode get_word(char *line, char *word, char ** next_word, int is_last_word);

/**
Extracts the next comma
   @param line: The input string.  
   @param next_word: Pointer to the rest of the line after the comma.  
   @return SUCCESS if a comma found, error otherwise.
*/
ErrorCode get_comma(char *line, char ** next_word);

/**
Parses a numeric operand and extracts its value.
	@param word: The operand string containing a number.
	@param value: A pointer to store the extracted numeric value.
	@return An error code indicating success or failure.
*/
ErrorCode get_number(char *word, int *value);

/**
Checks if a file name with path is too long.
   @param base: Base file name.
   @return 1 if file name is too long, 0 otherwise.
*/
int is_filename_too_long(char *base);

/**
Generates a file name with the given extension.  
   @param base: Base file name.  
   @param extension: File extension.
   @param filename: Output buffer for the full file name.
*/
void get_filename(char *base, char *extension, char *filename);

/**
Checks if a line exceeds the allowed length.  
   @param file: File pointer.  
   @param line: Buffer to check.  
   @return 1 if the line is too long, 0 otherwise.
*/
int is_line_too_long(FILE *file, char *line);

/**
Implement strdup since it is not defined for ANSI C
 */
char * my_strdup(char *string);

#endif /* UTILS_H */
