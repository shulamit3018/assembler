#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include "error_codes.h"

#define LINE_LEN 81 /* including zero termination */
#define MAX_FILE_NAME 128 /* Maximum length for a file name */

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
Opens a file with the given name and mode.  
   @param filename: Name of the file.  
   @param format: Opening mode (e.g., "r", "w").  
   @return pointer to the opened file, or NULL on failure.
*/
FILE * open_file (char *filename, char *format);

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

#endif /* UTILS_H */