#include <stdio.h>
#include <stdlib.h>
#include "error_codes.h"
#include "utils.h"
#include "macro.h"
#include "process.h"
#include "symbols.h"

/**
 * This program compiles an assembler file into machine code.
 * It performs several processing phases on the code:
 * 1. Macro Preprocessing:
 *    Performed in the function macro_process.
 *    Collects macro definitions and replaces their references with their content.
 * 2. Symbol Resolution:
 *    Performed in the function first_process.
 *    Collects labels and stores them as symbol definitions based on code & data memory addresses.
 * 3. Assembly:
 *    Performed in the function second_process.
 *    Resolves symbols and generates machine code.
 * Finally, the program outputs the machine code, as well as external and entry definitions, into files.
 * 
 * The program consists of several modules:
 * - Macro: Handles macro preprocessing.
 * - Process: Manages the main compilation logic.
 * - Assembler: Constructs the machine code for both code and data.
 * - Language: Defines instructions and syntax rules.
 * - Symbol: Manages the symbol table.
 * - Utils: Provides various utility functions.
 * - Error Codes: Handles error reporting.
 */
int main(int argc, char **argv)
{
    FILE *source, *destination;
	char source_filename[MAX_FILE_NAME];
	char destination_filename[MAX_FILE_NAME];
    int error = SUCCESS;
    int i;

	/* Check if at least one input file is provided */
    if (argc == 1)
    {
        printf("No input file provided.\n");
        exit(1);
    }

	/* Iterate over all input files */
    for (i = 1; i < argc; i++)
	{
		/* Open source file */
		if (is_filename_too_long(argv[i])) {
			is_error(ERR_FILE_NAME_TOO_LONG, NULL, argv[i], 0, NULL);
			continue;
		}
		get_filename(argv[i], "as", source_filename);
		printf("Building file %s...\n", source_filename);
		source = fopen(source_filename, "r");
		if (!source) {
			is_error(ERR_FILE_NOT_EXIST, NULL, source_filename, 0, NULL);
			continue;
		}

		/* Create file for processed macros */
		get_filename(argv[i], "am", destination_filename);
		destination = fopen(destination_filename, "w+");
		if (!destination) {
			is_error(ERR_FILE_CANNOT_CREATE, NULL, destination_filename, 0, NULL);
			continue;
		}

		/* Process macros and write the results */
		printf("Processing macros...\n");
		error = macro_process(source_filename, source, destination);
		fclose (source);
		fclose(destination);
		
		/* Error during macro processing: delete .am file and continue to next file */
		if (error) {
			unlink(destination_filename);
			purge_macros();
			continue;
		}

		/* First process: resolve symbols */
		get_filename(argv[i], "am", source_filename);
		source = fopen(source_filename, "r");
		if (!source) {
			is_error(ERR_FILE_NOT_EXIST, NULL, source_filename, 0, NULL);
			purge_macros();
			continue;
		}
		
		printf("Resolving symbols...\n");
		/* Error during first process: continue to next file */
		error = first_process(source_filename, source);
		if (error) {
			fclose (source);
			purge_macros();
			purge_symbols();
			continue;
		}

		/* Second process of the same file - seek to start */
		fseek (source, 0, SEEK_SET);
		printf("Assembling...\n");
		error = second_process(source_filename, source);
		/* Error during second process: do not create output files */
		if (error) {
			fclose (source);
			purge_macros();
			purge_symbols();
			continue;
		}

		fclose (source);
		
		/* Dump all files */
		printf("Generating output files...\n");
		get_filename(argv[i], "ob", destination_filename);
		destination = fopen(destination_filename, "w+");
		purge_and_dump_assembly(destination);
		fclose(destination);

		/* If external symbols exist, generate an extern file */
		if (has_extern()) {
			get_filename(argv[i], "ext", destination_filename);
			destination = fopen(destination_filename, "w+");
			dump_extern(destination);
			fclose(destination);
		}

		/* If entry symbols exist, generate an entry file */
		if (has_entry()) {
			get_filename(argv[i], "ent", destination_filename);
			destination = fopen(destination_filename, "w+");
			dump_entry(destination);
			fclose(destination);
		}

		/* Clean up stored macros and symbols before moving to the next file */
		purge_macros();
		purge_symbols();
		printf("Done file.\n");
	}

    return 0;
}
