#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "assemble.h"

#define IC_BASE 100
#define MEMORY_SIZE (1 << 21)
#define LINE_LEN 81

static int IC;
static int DC;

/* Code/data assembly table --------------------------------------------- */

/* A machine word node in list */ 
typedef struct assembly_node_t {
    assembly_t assembly;
    struct assembly_node_t *next;
} assembly_node_t;

/* Linked list of code/data nodes */
static assembly_node_t *code_head = NULL;
static assembly_node_t *code_tail = NULL;
static assembly_node_t *data_head = NULL;
static assembly_node_t *data_tail = NULL;

ErrorCode add_code(assembly_t assembly) {
    /* Allocate and insert a new node */
    assembly_node_t *node = (assembly_node_t *)malloc(sizeof(assembly_node_t));
    if (!node) {
        return ERR_OUT_OF_MEMORY;
    }
    node->assembly = assembly;
    node->next = NULL;

    /* If the list is empty, set both head and tail to the new node */
    if (!code_head) {
        code_head = node;
        code_tail = node;
    }
    /* Else, link the new node */
    else {
        code_tail->next = node;
        code_tail = node;
    }

    /* Increment the Instruction Counter */
    IC++;
    /* Check if the memory exceeds the maximum allowed size */
    if (is_exceeded_RAM()) {
        return ERR_EXCEEDED_RAM;
    }
    return SUCCESS;
}

ErrorCode add_data(assembly_t assembly) {
    /* Allocate and insert a new node */
    assembly_node_t *node = (assembly_node_t *)malloc(sizeof(assembly_node_t));
    if (!node) {
        return ERR_OUT_OF_MEMORY;
    }
    node->assembly = assembly;
    node->next = NULL;

    /* If the list is empty, set both head and tail to the new node */
    if (!data_head) {
        data_head = node;
        data_tail = node;
    }
    /* Else, link the new node */
    else {
        data_tail->next = node;
        data_tail = node;
    }

    /* Increment the Instruction Counter */
    DC++;
    /* Check if the memory exceeds the maximum allowed size */
    if (is_exceeded_RAM()) {
        return ERR_EXCEEDED_RAM;
    }
    return SUCCESS;
}
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

void reset_IC() {
    IC = IC_BASE;
}

void reset_DC() {
    DC = 0;
}

ErrorCode inc_IC(int n) {
    IC += n;
    /* Check if memory exceeds the limit after the increment */
    if (is_exceeded_RAM()) {
        return ERR_EXCEEDED_RAM;
    }
    return SUCCESS;
}

int get_IC() {
    return IC;
}

int get_DC() {
    return DC;
}

int is_exceeded_RAM() {
    return (IC + DC > MEMORY_SIZE);
}

void purge_and_dump_assembly(FILE *file) {
    assembly_node_t *current;
    char line[LINE_LEN];
    int address;

    /* Print the current IC and DC values */
    if (file) {
        sprintf(line, "%7d %-6d\n", IC - IC_BASE, DC);
        fputs(line, file);
    }
    
    /* Print the code section*/
    current = code_head;
    address = IC_BASE;
    while (current) {
        assembly_node_t *next = current->next;

        if (file) {
            sprintf(line, "%07d %06x\n", address, current->assembly.data.value);
            fputs(line, file);
        }

        free(current);
        current = next;
        address++;
    }
    /* Reset the code list */
    code_head = NULL;
    code_tail = NULL;

    /* Print the data section*/
    current = data_head;
    while (current) {
        assembly_node_t *next = current->next;

        if (file) {
            sprintf(line, "%07d %06x\n", address, current->assembly.data.value);
            fputs(line, file);
        }

        free(current);
        current = next;
        address++;
    }
    /* Reset the data list */
    data_head = NULL;
    data_tail = NULL;
}
