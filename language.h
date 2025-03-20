#ifndef LANGUAGE_H
#define LANGUAGE_H

#include "error_codes.h"

#define MAX_ADDRESSING 3
#define MAX_OPERANDS 2

/* Coding methods */
typedef enum {
	CODING_UNKNOWN = 0,
	CODING_E = 1,
	CODING_R = 2,
	CODING_A = 4
} coding_t;

/* Addressing methods */
typedef enum {
    IMMEDIATE = 0,
    DIRECT = 1,
    RELATIONAL = 2,
	REG = 3
} addressing_t;

/* Assembly instruction */
typedef struct instruction_t {
	char *name;
	int opcode;
	int funct;
	int number_of_operands;
	addressing_t allowed_addressing[MAX_OPERANDS][MAX_ADDRESSING];
} instruction_t;

/**
Checks if the given word is a valid assembly instruction.
	@param word The word to check.
 	@return 1 if the word is an instruction, 0 otherwise.
*/
int is_instruction(char *word);

/**
Retrieves the instruction structure associated with a given instruction name.
	@param name The instruction name.
 	@param instruction Pointer to store the retrieved instruction structure.
	@return ErrorCode indicating success or failure.
*/
ErrorCode get_instruction(char *name, instruction_t **instruction);

/**
Checks if the given word represents a register.
	@param word The word to check.
 	@param reg Pointer to store the register number if applicable.
 	@return 1 if the word is a register, 0 otherwise.
*/
int is_register(char *word, int *reg);

/**
Checks if the given word is a valid label.
	@param word The word to check.
	@return 1 if the word is a label, 0 otherwise.
*/
int is_label(char *word);

/**
Checks if the given word represents a data directive.
	@param word The word to check.
 	@return 1 if the word is a data directive, 0 otherwise.
*/
int is_data(char *word);

/**
Checks if the given word represents a string directive.
	@param word The word to check.
	@return 1 if the word is a string directive, 0 otherwise.
*/
int is_string(char *word);

/**
Checks if the given word is an "extern" directive.
	@param word The word to check.
	@return 1 if the word is an "extern" directive, 0 otherwise.
*/
int is_extern(char *word);

/**
Checks if the given word is an "entry" directive.
	@param word The word to check.
	@return 1 if the word is an "entry" directive, 0 otherwise.
*/
int is_entry(char *word);

/**
Checks if the given word is a reserved keyword in the assembly language.
	@param word The word to check.
	@return 1 if the word is reserved, 0 otherwise.
*/
int is_reserved_word(char *word);

/**
Validates if the given addressing method is allowed for an instruction operand.
	@param addressing The addressing method used.
	@param allowed_addressing The list of allowed addressing methods.
	@return 1 if the addressing is valid, 0 otherwise.
*/
int is_valid_addressing(addressing_t addressing, addressing_t allowed_addressing[]);

/**
Retrieves a formatted string describing the operand context based on operand index 
and total number of operands.
Used when reporting errors related to operands.
    @param i_operand The index of the operand being processed.
    @param n_operand The total number of expected operands.
    @return A string describing the operand position (e.g., "first operand").
*/
char *get_operand_context(int i_operand, int n_operand);

#endif /* LANGUAGE_H */