#include <stdio.h>
#include "error_codes.h"

char *get_operand_details(int i_operand, int n_operand) {
	/* If it is the first operand of two, it is the source operand */
	if (n_operand == 2 && i_operand == 0) {
		return "in source operand";
	}
	/* Otherwise, it is the destination operand */
	else {
		return "in destination operand";
	}
}

int is_error(ErrorCode error, int * error_state, char *filename, int line_number, char *error_context) {
	if (error == SUCCESS) {
		return 0;
	}

    if (error_state) {
	    *error_state = 1;
    }
	/* Print an error message */
	print_error(error, filename, line_number, error_context);
	return 1;
}

void print_error(ErrorCode error, char *filename, int line_number, char *error_context)
{
	/* Print error file & line */
	printf("error: %s", filename);
	if (line_number > 0) {
		printf(" line %d", line_number);
	}
	printf(": ");
	
	/* Print error details */
	switch(error)
	{
		case ERR_FILE_NOT_EXIST: printf("file does not exist"); break;
		case ERR_FILE_CANNOT_CREATE: printf("cannot create file"); break;
		case ERR_OUT_OF_MEMORY: printf("out of memory"); break;
		case ERR_INTERNAL_ASSERT: printf("internal error"); break;
		case ERR_COMMA_MISSING: printf("missing comma"); break;
		case ERR_COMMA_EXTRA: printf("extra comma"); break;
		case ERR_LINE_TOO_LONG: printf("line is too long"); break;
		case ERR_TRAILING_TEXT: printf("invalid trailing text"); break;
		case ERR_EXCEEDED_RAM: printf("RAM size exceeded"); break;

		/* Macro errors */
		case ERR_MACRO_REDEFINITION: printf("macro redefinition"); break;
		case ERR_MACRO_ILLEGAL_NAME: printf("illegal macro name"); break;
		case ERR_MACRO_NAME_TOO_LONG: printf("macro name is too long"); break;
		case ERR_MACRO_RESERVED: printf("macro name is a reserved word"); break;
		case ERR_MACRO_MISSING_NAME: printf("macro name is missing"); break;

		/* Symbol errors */
		case ERR_SYMBOL_ILLEGAL_NAME: printf("illegal symbol name"); break;
		case ERR_SYMBOL_NAME_TOO_LONG: printf("symbol name is too long"); break;
		case ERR_SYMBOL_REDEFINITION: printf("symbol redefinition"); break;
		case ERR_SYMBOL_UNDEFINED: printf("symbol is undefined"); break;
		case ERR_SYMBOL_RESERVED: printf("symbol name is a reserved word"); break;
		case ERR_SYMBOL_ILLEGAL: printf("illegal symbol on empty line"); break;
		case ERR_SYMBOL_AS_MACRO: printf("symbol and macro have the same name"); break;
		case ERR_SYMBOL_ENTRY_UNDEFINED: printf("illegal entry, symbol is undefined"); break;

		/* String errors */
		case ERR_STRING_ILLEGAL: printf("illegal string"); break;

		/* Operand errors */
		case ERR_OPERAND_MISSING: printf("missing operand"); break;

		/* Number errors */
		case ERR_NUMBER_ILLEGAL: printf("illegal number"); break;
		case ERR_NUMBER_OUT_OF_RANGE: printf("number is out of range"); break;

		/* Instruction errors */
		case ERR_INSTRUCTION_ADDRESSING_NOT_ALLOWED: printf("addressing method is not allowed"); break;
		case ERR_INSTRUCTION_INVALID: printf("invalid instruction"); break;

		default: printf("unknown error %d", error); break;
	}

	/* Print error context, If accepted */
	if (error_context) {
		printf(" %s", error_context);
	}
	printf("\n");
}
