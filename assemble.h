#ifndef ASSEMBLE_H
#define ASSEMBLE_H

#include <stdio.h>
#include "error_codes.h"
#include "language.h"

/* A machine word definition */ 
typedef union {
	struct{
		unsigned int ARE: 3;
        unsigned int funct: 5;
		unsigned int dst_reg: 3;
        unsigned int dst_addr: 2;
		unsigned int src_reg: 3;
        unsigned int src_addr: 2;
		unsigned int opcode: 6;
	} instruction;
	struct{
		unsigned int ARE: 3;
		signed int value: 21;
	} operand;
    struct {
        unsigned int value: 24;
    } data;    
} assembly_t;

/** 
Adds machine code (assembly instruction) to the code section.
	@param assembly: The assembly_t object containing the instruction to add.
	@return: Error code indicating success or failure.
*/
ErrorCode add_code(assembly_t assembly);

/** 
Adds data to the data section.
	@param assembly: The assembly_t object containing the data to add.
	@return: Error code indicating success or failure.
*/
ErrorCode add_data(assembly_t assembly);

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
Resets the Instruction Counter (IC) to the initial value.
*/
void reset_IC();

/** 
Resets the Data Counter (DC) to the initial value.
*/
void reset_DC();

/** 
Increments the Instruction Counter (IC) by `n`.
	@param n: The number by which to increment the IC.
	@return: The updated value of the IC after increment.
*/
ErrorCode inc_IC(int n);

/** 
Returns the current value of the Instruction Counter (IC).
	@return: The current value of the IC.
*/
int get_IC();

/** 
Returns the current value of the Data Counter (DC).
	@return: The current value of the DC.
*/
int get_DC();

/** 
Checks if the system's memory (RAM) has been exceeded.
	@return: 1 if RAM is exceeded, 0 otherwise.
*/
int is_exceeded_RAM();

/** 
Dumps the assembly code to a specified file.
	@param file: The file pointer where the assembly code will be written.
*/
void purge_and_dump_assembly(FILE *file);

#endif /* ASSEMBLE_H */
