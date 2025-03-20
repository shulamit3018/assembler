#ifndef SYMBOLS_H
#define SYMBOLS_H

#include <stdio.h>
#include "error_codes.h"

/* The storage type of a symbol */
typedef enum {
    CODE,
    DATA,
    EXTERN
} storage_t;

/**
Adds a new symbol to the symbol table
    @param name: The symbol name.
    @param address: Address of the symbol.
    @param storage: Symbol type (CODE, DATA, EXTERN).
    @param is_entry: Nonzero if the symbol is an entry.
    @return SUCCESS if added successfully, error otherwise.
*/
ErrorCode add_symbol(char *name, int address, storage_t storage, int is_entry);

/**
Retrieves the address and storage type of a symbol
    @param name Symbol name.
    @param address Pointer to store the retrieved address.
    @param storage Pointer to store the retrieved storage type.
    @return SUCCESS if a symbol found, error otherwise.
*/
ErrorCode get_symbol(char *name, int *address, storage_t *storage);

/**
Marks a symbol as an entry
    @param name Symbol name.
    @return SUCCESS if successful, error otherwise.
*/
ErrorCode set_symbol_entry(char *name);

/**
Records a reference to an external symbol
    @param name Symbol name.
    @param address Address where the symbol is referenced.
*/
void add_external_symbol_references(char *name, int address);

/** Adjusts symbols' addresses based on the instruction counter (IC). */
void fix_symbols_by_IC();

/**
Checks if there are any external symbols in the symbol table.
    @return 1 if there are external symbols, 0 otherwise.
*/
int has_extern();

/**
Checks if there are any entry symbols in the symbol table.
    @return 1 if there are entry symbols, 0 otherwise.
*/
int has_entry();

/**
Writes all external symbols and their references to a file.
    @param file File pointer to write to.
*/
void dump_extern(FILE* file);

/**
Writes all entry symbols to a file.
    @param file File pointer to write to.
*/
void dump_entry(FILE* file);

/**
Clears all stored symbols from the symbol table.
*/
void purge_symbols();

#endif /* SYMBOLS_H */
