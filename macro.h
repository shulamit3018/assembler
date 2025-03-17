#ifndef MACRO_H
#define MACRO_H

#include <stdio.h>
#include "error_codes.h"

/**
Processes macros in the given source file and writes the expanded output to the destination file.
   @param filename: The name of the source file being processed.
   @param source: A pointer to the source file.
   @param destination: A pointer to the destination file where the processed output will be written.
   @return 0 on success, 1 on failure.
*/
int macro_process(char *filename, FILE *source, FILE *destination);

/**
Adds a new macro definition with the given name.
   @param name: The name of the macro to be added.
   @return SUCCESS if added successfully, error otherwise.
*/
ErrorCode add_macro(char *name);

/**
Adds content to an existing macro.
   @param name: The name of the macro.
   @param content: The content to be added to the macro.
   @return SUCCESS if content was added, error otherwise.
*/
ErrorCode add_macro_content(char *name, char *content);

/**
Checks if a given name corresponds to a defined macro.
   @param name: The name to check.
   @return 1 if the name is a macro, 0 otherwise.
*/
int is_macro(char *name);

/**
Retrieves the content of a macro by name.
   @param name: The name of the macro.
   @param content: Pointer to a string where the macro content will be stored.
   @return SUCCESS if the macro was found, error otherwise.
*/
ErrorCode get_macro(char *name, char **content);

/**
Frees all allocated memory for macros and clears the macro table.
*/
void purge_macros();

#endif /* MACRO_H */
