#ifndef PROCESS_H
#define PROCESS_H

#include <stdio.h>
#include "error_codes.h"
#include "language.h"
#include "assemble.h"

/**
First processing phase on the given file:
Scans the file, collects labels, and determines memory addresses.
	@param filename: The name of the file being processed.  
	@param file: A pointer to the opened file for processing.  
	@return 0 on success, 1 on failure.
*/ 
int first_process(char *filename, FILE *file);

/**
Second processing phase on the given file:
Resolves symbols and generates machine code.
	@param filename: The name of the file being processed.  
	@param file: A pointer to the opened file for processing.  
	@return 0 on success, 1 on failure.  
*/  
int second_process(char *filename, FILE *file);

/**
Determines the length of an instruction based on its operands.  
	@param name: The instruction name.  
	@param rest_of_line: The remaining part of the line after the instruction.  
	@param n_operands: A pointer to store the number of operands.  
	@return An error code indicating success or failure.  
*/  
ErrorCode get_instruction_length(char *name, char *rest_of_line, int *n_operands);

/**
Processes an instruction and extracts its operands.
	@param word: The instruction name.
	@param rest_of_line: The remaining part of the line after the instruction.
	@param assembly: A pointer to the assembly structure storing the instruction.
	@param operands: A pointer to the structure storing operands.
	@param n_operands: A pointer to store the number of operands.
	@param error_context: A string containing additional context for error messages.
	@return An error code indicating success or failure.
*/
ErrorCode process_instruction(
	char *word, 
	char *rest_of_line, 
	assembly_t *assembly, 
	assembly_t *operands, 
	int *n_operands,
	char *error_context
);

/**
Processes an operand and determines its addressing mode and IC value.  
	@param operand: The operand string.
	@param i_operand: The index of the operand in the current instruction.
	@param allowed_addressing: An array of allowed addressing modes for the operand.
	@param value: A pointer to store the resolved operand value.
	@param are: A pointer to store the ARE (Absolute, Relocatable, External) value.
	@param addressing: A pointer to store the determined addressing mode.
	@return An error code indicating success or failure.
*/
ErrorCode process_operand(
	char *operand, 
	int i_operand, 
	addressing_t allowed_addressing[], 
	int *value, 
	coding_t *are, 
	addressing_t *addressing
);

/**
Sets the register value for an operand.
	@param assembly: A pointer to the assembly structure.
	@param reg: The register number.
	@param i_operand: The index of the operand in the current instruction.
	@param number_of_operands: The total number of operands in the instruction.
*/
void set_reg(assembly_t *assembly, int reg, int i_operand, int number_of_operands);

/**
Sets the addressing mode for an operand.
	@param assembly: A pointer to the assembly structure.
	@param addressing: The determined addressing mode.
	@param i_operand: The index of the operand in the current instruction.
	@param number_of_operands: The total number of operands in the instruction.
*/
void set_addressing(assembly_t *assembly, int addressing, int i_operand, int number_of_operands);

/**
Parses a numeric operand and extracts its value.
	@param word: The operand string containing a number.
	@param value: A pointer to store the extracted numeric value.
	@return An error code indicating success or failure.
*/
ErrorCode get_number(char *word, int *value);

#endif /* PROCESS_H */
