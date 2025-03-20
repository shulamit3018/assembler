#ifndef ERROR_CODES_H
#define ERROR_CODES_H

/* Possible errors */
typedef enum {
    SUCCESS,

    /* General errors */
    ERR_FILE_NOT_EXIST,
    ERR_FILE_NAME_TOO_LONG,
    ERR_FILE_CANNOT_CREATE,
    ERR_OUT_OF_MEMORY,
    ERR_INTERNAL_ASSERT,
    ERR_COMMA_MISSING,
    ERR_COMMA_EXTRA,
    ERR_LINE_TOO_LONG,
    ERR_TRAILING_TEXT,
    ERR_EXCEEDED_RAM,

    /* Macro errors */
    ERR_MACRO_REDEFINITION,
    ERR_MACRO_ILLEGAL_NAME,
    ERR_MACRO_NAME_TOO_LONG,
    ERR_MACRO_RESERVED,
    ERR_MACRO_MISSING_NAME,

    /* Symbol errors */
    ERR_SYMBOL_ILLEGAL_NAME,
    ERR_SYMBOL_NAME_TOO_LONG,
    ERR_SYMBOL_REDEFINITION,
    ERR_SYMBOL_UNDEFINED,
    ERR_SYMBOL_RESERVED,
    ERR_SYMBOL_ILLEGAL,
    ERR_SYMBOL_AS_MACRO,
    ERR_SYMBOL_ENTRY_UNDEFINED,
    
    /* String errors */
    ERR_STRING_ILLEGAL,

    /* Operand errors */
    ERR_OPERAND_MISSING,

    /* Number errors */
    ERR_NUMBER_ILLEGAL,
    ERR_NUMBER_OUT_OF_RANGE,

    /* Instruction errors */
    ERR_INSTRUCTION_ADDRESSING_NOT_ALLOWED,
    ERR_INSTRUCTION_INVALID

} ErrorCode;

/**
Prints a detailed error message based on the given error code.
    @param error The error code.
    @param filename The source file where the error occurred.
    @param line_number The line number where the error was detected.
    @param error_context Additional context.
*/
void print_error(ErrorCode error, char *filename, int line_number, char *error_context);

/**
Checks if an error occurred and updates the error state if applicable.
    @param error The error code.
    @param error_state Pointer to an error tracking variable.
    @param filename The source file where the error occurred.
    @param line_number The line number where the error was detected.
    @param error_context Additional context (e.g., symbol, macro, or operand name).
    @return 1 if an error occurred, 0 otherwise.
*/
int is_error(ErrorCode error, int * error_state, char *filename, int line_number, char *error_context);

#endif /* ERROR_CODES_H */