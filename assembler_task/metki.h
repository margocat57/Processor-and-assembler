#ifndef METKI_H
#define METKI_H
// #include "assembler_struct.h"

struct assembler;

const int MAX_NUMBER_OF_METKI = 10;

struct metki{
    int* metki_arr;
};

//! @brief Initializes metki array 
//!
//! This function scans through all assembly instructions to identify label definitions
//! and creates a mapping from label numbers to their corresponding program counter (PC) values.
//!
//! Operation:
//! 1. Validates assembler pointer is not NULL
//! 2. Allocates and zero-initializes label array with MAX_NUMBER_OF_METKI elements
//! 3. Scans all instructions to find label definitions (marked with pc == -1)
//! 4. Extracts label number from instruction string (format: "label:NUMBER")
//! 5. Maps label to the PC value of the NEXT instruction (idx + 1)
//! 6. Stores label-to-PC mapping in the metki array
//!
//! @note Calls metki_dump() for debugging output after initialization
//! @note Returns empty structure if allocation fails or assembler pointer is NULL
metki metki_init(struct assembler* assembl);

void metki_destroy(assembler* assembl);

#endif //METKI_H