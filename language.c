#include <string.h>
#include <ctype.h>
#include "language.h"

/* Array of supported instructions,
   including allowed addressing methods for each operand */
instruction_t instructions[] = {
	{ "mov", 0, 0, 2, {{IMMEDIATE, DIRECT, REG}, {DIRECT, REG}}},
	{ "cmp", 1, 0, 2, {{IMMEDIATE, DIRECT, REG}, {IMMEDIATE, DIRECT, REG}}},
	{ "add", 2, 1, 2, {{IMMEDIATE, DIRECT, REG}, {DIRECT, REG}}},
	{ "sub", 2, 2, 2, {{IMMEDIATE, DIRECT, REG}, {DIRECT, REG}}},
	{ "lea", 4, 0, 2, {{DIRECT}, {DIRECT, REG}}},
	{ "clr", 5, 1, 1, {{DIRECT, REG}}},
	{ "not", 5, 2, 1, {{DIRECT, REG}}},
	{ "inc", 5, 3, 1, {{DIRECT, REG}}},
	{ "dec", 5, 4, 1, {{DIRECT, REG}}},
	{ "jmp", 9, 1, 1, {{DIRECT, RELATIONAL}}},
	{ "bne", 9, 2, 1, {{DIRECT, RELATIONAL}}},
	{ "jsr", 9, 3, 1, {{DIRECT, RELATIONAL}}},
	{ "red", 12, 0, 1, {{DIRECT, REG}}},
	{ "prn", 13, 0, 1, {{IMMEDIATE, DIRECT, REG}}},
	{ "rts", 14, 0, 0 },
	{ "stop", 15, 0, 0 }
};

int is_instruction(char *word) {
	int i;
	int n = sizeof(instructions)/sizeof(instruction_t);
	/* Search for instruction in the array */
	for (i = 0; i < n; i++) {
		if (strcmp(word,instructions[i].name) == 0) {
			return 1;
		}
	}
	return 0;
}

ErrorCode get_instruction(char *name, instruction_t **instruction) {
	int i;
	int n = sizeof(instructions)/sizeof(instruction_t);
	/* Search for instruction in the array */
	for (i = 0; i < n; i++) {
		if (strcmp(name, instructions[i].name) == 0) {
			*instruction = &instructions[i];
			return SUCCESS;
		}
	}
	return ERR_INSTRUCTION_INVALID;
}

int is_label(char *word) {
	return word[strlen(word) -1] == ':';
}

int is_data(char *word) {
	return strcmp(word, ".data") == 0;
}

int is_string(char *word) {
	return strcmp(word, ".string") == 0;
}

int is_extern(char *word) {
	return strcmp(word, ".extern") == 0;
}

int is_entry(char *word) {
	return strcmp(word, ".entry") == 0;
}

int is_reserved_word(char *word) {
	return 
		is_instruction(word) ||
		is_register(word, NULL) ||
		strcmp(word, "data") == 0 ||
		strcmp(word, "string") == 0 ||
		strcmp(word, "extern") == 0 ||
		strcmp(word, "entry") == 0 ||
		strcmp(word, "mcro") == 0;
}

int is_register(char *word, int *reg) {
	if (
		/* Ensure the register name is exactly two characters: 'r' followed by a digit */
		strlen(word) == 2 && 
		word[0] == 'r' && 
		isdigit(word[1]) && 
		word[1] >= '0' 
		&& word[1] <= '7' 
		&& word[2] == 0
	) {
		if (reg) {
			/* Convert character digit to integer and store in reg */
			*reg = word[1] - '0';
		}
		return 1;
	}
	return 0;
}

int is_valid_addressing(addressing_t addressing, addressing_t allowed_addressing[]) {
	int i;
	for (i = 0; i <= MAX_ADDRESSING; i++) {
		if (allowed_addressing[i] == addressing) {
			return 1;
		}
	}
	return 0;
}

char *get_operand_context(int i_operand, int n_operand) {
	/* If it is the first operand of two, it is the source operand */
	if (n_operand == 2 && i_operand == 0) {
		return "in source operand";
	}
	/* Otherwise, it is the destination operand */
	else {
		return "in destination operand";
	}
}
