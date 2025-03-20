#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "utils.h"
#include "language.h"
#include "macro.h"
#include "symbols.h"

/* Symbol table --------------------------------------------- */

#define SYMBOL_LEN 31

/* A symbol definition */ 
typedef struct symbol_t {
	char *name;
	int address;
	storage_t storage;
    int is_entry;
    struct symbol_t *next;
} symbol_t;

/* Linked list of symbols */
static symbol_t *symbol_list_head = NULL;
static symbol_t *symbol_list_tail = NULL;

/* External symbols usage */
char *external_symbol_references;

ErrorCode add_symbol(char *name, int address, storage_t storage, int is_entry) {
	int i;
	symbol_t *symbol;

    /* Validate symbol name */
    /* Check if the symbol name is a reserved word */
    if (is_reserved_word(name)) {
        return ERR_SYMBOL_RESERVED;
    }
    
    /* Check if this is a macro name */
    if (is_macro(name)) {
        return ERR_SYMBOL_AS_MACRO;
    }
    
    /* Ensure the symbol name is within the allowed length */
    if (strlen(name) > SYMBOL_LEN) {
        return ERR_SYMBOL_NAME_TOO_LONG;
    }

    /* Ensure the symbol name starts with a letter */
    if (!isalpha(name[0])) {
        return ERR_SYMBOL_ILLEGAL_NAME;
    }

    /* Ensure that the rest characters in the name are alphanumeric */
	for (i = 1; i < strlen(name); i++) {
		if (!isalpha(name[i]) && !isdigit(name[i])) {
			return ERR_SYMBOL_ILLEGAL_NAME;
		}
	}
    
    /* If this is just a symbol reference (and not a definition), do not add */
    if (address < 0) {
        return SUCCESS;
    }

    /* Check if symbol already defined */
    if (get_symbol(name, NULL, NULL) == SUCCESS) {
        return ERR_SYMBOL_REDEFINITION;
    }

    /* Allocate and insert a new symbol */
    symbol = (symbol_t *)malloc(sizeof(symbol_t));
    if (!symbol) {
        return ERR_OUT_OF_MEMORY;
    }

    symbol->name = my_strdup(name);
	symbol->address = address;
    symbol->storage = storage;
    symbol->is_entry = is_entry;
    symbol->next = NULL;

    /* Insert symbol into the linked list */
    if (!symbol_list_head) {
        symbol_list_head = symbol;
        symbol_list_tail = symbol;
    }
    else {
        symbol_list_tail->next = symbol;
        symbol_list_tail = symbol;
    }

    return SUCCESS;
}

ErrorCode get_symbol(char *name, int *address, storage_t *storage) {
    symbol_t *current = symbol_list_head;
    /* Search for the symbol in the symbol list. */
    while (current) {
        if (strcmp(current->name, name) == 0) {
            /* Set the address if requested */
            if (address) {
                *address = current->address;
            }
            /* Set the storage type if requested */
            if (storage) {
                *storage = current->storage;
            }
            return SUCCESS;
        }
        current = current->next;
    }
    return ERR_SYMBOL_UNDEFINED;
}

ErrorCode set_symbol_entry(char *name) {
    symbol_t *current = symbol_list_head;
    /* Search for symbol in the symbol list. */
    while (current) {
        if (strcmp(current->name, name) == 0) {
            /* Set the symbol as an entry point */
            current->is_entry = 1;
            return SUCCESS;
        }
        current = current->next;
    }
    return ERR_SYMBOL_ENTRY_UNDEFINED;
}

void add_external_symbol_references(char *name, int address) {
    char line[LINE_LEN];
    /* Format the external symbol reference */
    sprintf(line, "%s %07d\n", name, address);

    /* Allocate memory for the first reference */
    if (!external_symbol_references) {
        external_symbol_references = (char *)malloc(strlen(line) + 1);
        strcpy(external_symbol_references, line);
    }
    /* Append the new reference to the existing list*/
    else {
        external_symbol_references = (char *)realloc(external_symbol_references, strlen(external_symbol_references) + strlen(line) + 1);
        strcat(external_symbol_references, line);
    }
}

void fix_symbols_by_IC(int IC) {
    symbol_t *current = symbol_list_head;
    /* Traverse the symbol list and adjust the address of data symbols */
    while (current) {
        if (current->storage == DATA) {
            /* Adjust the address by adding the IC */
            current->address += IC;
        }
        current = current->next;
    }
}

int has_extern() {
    return external_symbol_references != NULL;
}

int has_entry() {
    symbol_t *current = symbol_list_head;
    while (current) {
        if (current->is_entry) {
            return 1;
        }
        current = current->next;
    }
    return 0;
}

void dump_extern(FILE* file) {
    /* Write external references to the file */
    fputs(external_symbol_references, file);
}

void dump_entry(FILE* file) {
    char line[LINE_LEN];
    symbol_t *current = symbol_list_head;
    while (current) {
        if (current->is_entry) {
            /* Format the entry symbol */
            sprintf(line, "%s %07d\n", current->name, current->address);
            /* Write the entry symbol to the file */
            fputs(line, file);
        }
        current = current->next;
    }
}

void purge_symbols() {
    symbol_t *current = symbol_list_head;
    /* Free the symbol table*/
    while (current) {
        symbol_t *next = current->next;
        free(current->name);
        free(current);
        current = next;
    }
    symbol_list_head = NULL;
    symbol_list_tail = NULL;

    /* Free memory for external references */
    free(external_symbol_references);
    external_symbol_references = NULL;
}
