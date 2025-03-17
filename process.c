#include <string.h>
#include <stdlib.h>
#include "process.h"
#include "utils.h"
#include "symbols.h"

int first_process(char *filename, FILE *file)
{
	int line_number = 0;
	char line[LINE_LEN];
	char word[LINE_LEN];
	char label[LINE_LEN];
	int value;
	char *rest_of_line;
	assembly_t assembly;
	int i; /* loop index */

	ErrorCode error;
	int error_state = 0;

	reset_IC();
	reset_DC();
	
	/* Read file line by line */
	while(fgets(line, LINE_LEN, file))
	{
		line_number++;
		label[0] = '\0';
		
		/* Ensure null termination */
		line[LINE_LEN-1] = '\0';
		rest_of_line = line;

        /* Skip empty lines and comments */
		if (line[0] == ';' || is_whitespaces(line)) {
			continue;
		}

		/* Extract the first word in the line */
		error = get_word(rest_of_line, word, &rest_of_line, LAST_WORD_DONT_CARE);
		if (is_error(error, &error_state, filename, line_number, NULL)) {
			continue;
		}

		/* If the first word is a label, store it and extract the next word */
		if (is_label(word)) {
			strcpy(label, word);
			label[strlen(label) - 1] = '\0'; /* Remove colon */
			error = get_word(rest_of_line, word, &rest_of_line, LAST_WORD_DONT_CARE);
			if (is_error(error, &error_state, filename, line_number, NULL)) {
				continue;
			}
			if (strlen(word) == 0) {
				is_error(ERR_SYMBOL_ILLEGAL, &error_state, filename, line_number, NULL);
				continue;
			}
		}

		/* Process different types of assembler directives and instructions */

		/* Handle .data directive */
		if (is_data(word)) {
			if (*label) {
				error = add_symbol(label, get_DC(), DATA, 0);
				if (is_error(error, &error_state, filename, line_number, NULL)) {
					continue;
				}
			}

			/* Parse and store numeric values */
			do {
				error = get_word(rest_of_line, word, &rest_of_line, LAST_WORD_DONT_CARE);
				if (is_error(error, &error_state, filename, line_number, NULL)) {
					continue;
				}

				error = get_number(word, &value);
				if (is_error(error, &error_state, filename, line_number, NULL)) {
					continue;
				}

				assembly.data.value = value;
				error = add_data(assembly);
				if (is_error(error, &error_state, filename, line_number, NULL)) {
					/* Out of memory or RAM exceeded - do not coninue this file*/
					return error_state;
				}
	
				if (rest_of_line && !is_whitespaces(rest_of_line)) {
					error = get_comma(rest_of_line, &rest_of_line);
				}
			} while (*rest_of_line && error == SUCCESS);

		}
		/* Handle .string directive */
		else if (is_string(word)) {
			if (*label) {
				error = add_symbol(label, get_DC(), DATA, 0);
				if (is_error(error, &error_state, filename, line_number, NULL)) {
					continue;
				}
			}

			/* Extract string content */
			error = get_word(rest_of_line, word, &rest_of_line, LAST_WORD);
			if (is_error(error, &error_state, filename, line_number, NULL)) {
				continue;
			}
			/* Validate string format */
			if (strlen(word) < 2 || word[0] != '"' || word[strlen(word) - 1] != '"') {
				is_error(ERR_STRING_ILLEGAL, &error_state, filename, line_number, NULL);
				continue;
			}
			/* Store characters in memory */
			for (i = 1; i < strlen(word) - 1; i++) {
				assembly.data.value = word[i];
				error = add_data(assembly);
				if (is_error(error, &error_state, filename, line_number, NULL)) {
					/* Out of memory or RAM exceeded - do not coninue this file*/
					return error_state;
				}
			}

			/* Add zero termination */
			assembly.data.value = 0;
			error = add_data(assembly);
			if (is_error(error, &error_state, filename, line_number, NULL)) {
				/* Out of memory or RAM exceeded - do not coninue this file*/
				return error_state;
			}
		}
		/* Handle .extern directive */
		else if (is_extern(word)) {
			error = get_word(rest_of_line, word, &rest_of_line, LAST_WORD);
			if (is_error(error, &error_state, filename, line_number, NULL)) {
				continue;
			}

			error = add_symbol(word, 0, EXTERN, 0);
			if (is_error(error, &error_state, filename, line_number, NULL)) {
				continue;
			}
		}
		/* Entry is processed on second scan */
		else if (is_entry(word)) {
			continue;
		}
		/* Handle an assembly instruction */
		else if (is_instruction(word)) {
			int n_operands;

			if (*label) {
				error = add_symbol(label, get_IC(), CODE, 0);
				if (is_error(error, &error_state, filename, line_number, NULL)) {
					continue;
				}
			}

			error = get_instruction_length(word, rest_of_line, &n_operands);
			if (is_error(error, &error_state, filename, line_number, NULL)) {
				continue;
			}

			/* Check memory capacity */
			error = inc_IC(1 + n_operands);
			if (is_error(error, &error_state, filename, line_number, NULL)) {
				break;
			}
		}
		/* If the word does not match any valid directive or instruction, report an error */
		else {
			is_error(ERR_INSTRUCTION_INVALID, &error_state, filename, line_number, NULL);
		}
	}

	/* Adjust value of symbols based on IC */
	fix_symbols_by_IC(get_IC());

	return error_state;
}

int second_process(char *filename, FILE *file)
{
	int line_number = 0;
	char line[LINE_LEN];
	char word[LINE_LEN];
	char *rest_of_line;
	assembly_t assembly;
	int i;

	ErrorCode error;
	int error_state = 0;
	
	/* Reset instruction counter before second scan */
	reset_IC();
	
	/* Read file line by line */
	while(fgets(line, LINE_LEN, file))
	{
		line_number++;
		
		/* Ensure null termination */
		line[LINE_LEN-1] = '\0';
		rest_of_line = line;

		/* Skip empty lines and comments */
		if (line[0] == ';' || is_whitespaces(line)) {
			continue;
		}

		/* Extract the first word in the line */
		error = get_word(rest_of_line, word, &rest_of_line, LAST_WORD_DONT_CARE);
		if (is_error(error, &error_state, filename, line_number, NULL)) {
			continue;
		}

		/* Labels are already processed on first scan */
		if (is_label(word)) {
			/* Skip to next word */
			get_word(rest_of_line, word, &rest_of_line, LAST_WORD_DONT_CARE);
		}

		/* Data, string & extern are already processed on first scan */
		if (is_data(word) || is_string(word) || is_extern(word)) {
			continue;
		}

		/* Process .entry directive */
		else if (is_entry(word)) {
			error = get_word(rest_of_line, word, &rest_of_line, LAST_WORD);
			if (is_error(error, &error_state, filename, line_number, NULL)) {
				continue;
			}

			error = set_symbol_entry(word);
			if (is_error(error, &error_state, filename, line_number, NULL)) {
				continue;
			}
		}

		/* Process an instruction */
		else if (is_instruction(word)) {
			char error_context[DETAILS_LEN];
			assembly_t operands[2];
			int n_operands;

			/* Parse the instruction and extract operands */
			error = process_instruction(word, rest_of_line, &assembly, operands, &n_operands, error_context);
			if (is_error(error, &error_state, filename, line_number, error_context)) {
				continue;
			}

			/* Add the parsed instruction to the code section */
			error = add_code(assembly);
			if (is_error(error, &error_state, filename, line_number, NULL)) {
				/* Out of memory or RAM exceeded - do not coninue this file*/
				return error_state;
			}
			/* Process and add each operand to the code section */
			for (i = 0; i < n_operands; i++) {
				error = add_code(operands[i]);
				if (is_error(error, &error_state, filename, line_number, NULL)) {
					/* Out of memory or RAM exceeded - do not coninue this file*/
					return error_state;
				}
			}
		}
		/* If the word does not match any valid directive or instruction, report an error */
		else {
			is_error(ERR_INSTRUCTION_INVALID, &error_state, filename, line_number, NULL);
		}
	}

	return error_state;
}

ErrorCode get_instruction_length(char *name, char *rest_of_line, int *n_operands) {
	ErrorCode error;
	instruction_t *instruction;
	char word[LINE_LEN];
	int i;

	/* Ensure the operand counter pointer is valid */
	if (!n_operands) {
		return ERR_INTERNAL_ASSERT;
	}

	/* Retrieve the instruction details */
	error = get_instruction(name, &instruction);
	if (error != SUCCESS) {
		return error;
	}

	*n_operands = 0;
	
	/* Count operands */
	for (i = 0; i < instruction->number_of_operands; i++) {
		error = get_word(rest_of_line, word, &rest_of_line, i == instruction->number_of_operands - 1);
		if (error != SUCCESS) {
			return error;
		}
		if (strlen(word) == 0) {
			return ERR_OPERAND_MISSING;
		}

		/* Registers are not counted as operands */
		if (!is_register(word, NULL)) {
			(*n_operands)++;
		}

		/* Ensure correct operand separation */
		if (i < instruction->number_of_operands - 1) {
			error = get_comma(rest_of_line, &rest_of_line);
			if (error != SUCCESS) {
				return error;
			}
		}
	}
	
	return SUCCESS;
}

ErrorCode process_instruction(
	char *name, 
	char *rest_of_line, 
	assembly_t *assembly,
	assembly_t *operands, 
	int *n_operands,
	char *error_context
) {
	ErrorCode error;
	instruction_t *instruction;
	char word[LINE_LEN];
	int value;
	coding_t are;
	addressing_t addressing;
	int reg;
	int i;

	/* Ensure valid pointers */
	if (!assembly || !operands || !n_operands) {
		return ERR_INTERNAL_ASSERT;
	}

	/* Retrieve instruction details */
	error = get_instruction(name, &instruction);
	if (error != SUCCESS) {
		return error;
	}

	/* Initialize assembly structure */
	assembly->data.value = 0;
	*n_operands = 0;
	
	/* Set opcode and function values */
	assembly->instruction.ARE = CODING_A;
	assembly->instruction.opcode = instruction->opcode;
	assembly->instruction.funct = instruction->funct;

	/* Process operands */
	for (i = 0; i < instruction->number_of_operands; i++) {
		if (error_context) {
			strcpy(error_context, get_operand_details(i, instruction->number_of_operands));
		}

		/* Extract operand */
		error = get_word(rest_of_line, word, &rest_of_line, instruction->number_of_operands == 1);
		if (error != SUCCESS) {
			return error;
		}
		if (strlen(word) == 0) {
			return ERR_OPERAND_MISSING;
		}

		/* Handle register operands */
		if (is_register(word, &reg)) {
            /* Ensure register addressing is allowed for this operand */
			if (!is_valid_addressing(REG, instruction->allowed_addressing[i])) {
				return ERR_INSTRUCTION_ADDRESSING_NOT_ALLOWED;
			}
			set_reg(assembly, reg, i, instruction->number_of_operands);
		}
		/* Handle non-register operands */
		else {
			error = process_operand(word, i, instruction->allowed_addressing[i], &value, &are, &addressing);
			if (error != SUCCESS) {
				return error;
			}

			/* If it is an external referece, store the address for later resolution */
			if (are == CODING_E) {
				add_external_symbol_references(word, get_IC() + i + 1);
			}

			operands[*n_operands].operand.ARE = are;
			operands[*n_operands].operand.value = value;
			(*n_operands)++;

			set_addressing(assembly, addressing, i, instruction->number_of_operands);
		}

		 /* Ensure correct operand separation */
		if (i < instruction->number_of_operands - 1) {
			error = get_comma(rest_of_line, &rest_of_line);
			if (error != SUCCESS) {
				return error;
			}
		}
	}
	
	return SUCCESS;
}

ErrorCode process_operand(
	char *operand, 
	int i_operand, 
	addressing_t allowed_addressing[], 
	int *value, 
	coding_t *are, 
	addressing_t *addressing
) {
	ErrorCode error;
	storage_t storage;

	/* Immediate addressing */
	if (*operand == '#') {
		error = get_number(operand+1, value);
		if (error != SUCCESS) {
			return error;
		}
		*are = CODING_A;
		*addressing = IMMEDIATE;
	}

	/* Relational addressing */
	else if (*operand == '&') {
		int address;
		error = get_symbol(operand+1, &address, &storage);
		if (error != SUCCESS) {
			return ERR_SYMBOL_UNDEFINED;
		}

		*value = address - get_IC();
		*are = CODING_A;
		*addressing = RELATIONAL;
	}

	/* Direct addressing */
	else {
		error = get_symbol(operand, value, &storage);
		if (error != SUCCESS) {
			return ERR_SYMBOL_UNDEFINED;
		}

		/* Determine whether the symbol is external or internal */
		if (storage == EXTERN) {
			*are = CODING_E;
		}
		else {
			*are = CODING_R;
		}
		*addressing = DIRECT;
	}

	/* Ensure the addressing mode is allowed */
	if (!is_valid_addressing(*addressing, allowed_addressing)) {
		return ERR_INSTRUCTION_ADDRESSING_NOT_ALLOWED;
	}

	return SUCCESS;
}

/* Helper Functions ------------------------------------------------- */

void set_reg(assembly_t *assembly, int reg, int i, int number_of_operands) {
	/* With two operands, the first one is osource, otherwise it is destination */
	if (number_of_operands == 2 && i == 0) {
		assembly->instruction.src_reg = reg;
		assembly->instruction.src_addr = REG;
	}
	else {
		assembly->instruction.dst_reg = reg;
		assembly->instruction.dst_addr = REG;
	}
}

void set_addressing(assembly_t *assembly, int addressing, int i, int number_of_operands) {
	/* With two operands, the first one is osource, otherwise it is destination */
	if (number_of_operands == 2 && i == 0) {
		assembly->instruction.src_addr = addressing;
	}
	else {
		assembly->instruction.dst_addr = addressing;
	}
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



