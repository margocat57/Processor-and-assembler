#ifndef PARSE_ASM_FROM_FILE_H
#define PARSE_ASM_FROM_FILE_H
#include "processor.h"

//! @brief Loads and validates bytecode from binary file
//!
//! Operation Sequence:
//! 1. Opens file in binary read mode
//! 2. Reads and verifies author signature  BYTECODE_AUTOR_STR
//! 3. Reads the size of the bytecode array (as size_t)
//! 4. Allocates memory for the bytecode array (+1 for safety margin)
//! 5. Reads the actual bytecode data into the allocated array
//! 6. Returns inicialized code_and_size structure on success
//!
//! Error Handling:
//! - File opening failures
//! - Signature mismatches (invalid file format)
//! - Read operation failures
//! - Memory allocation failures
//! - All errors result in empty structure return and closed file
//! @note Returns zero-initialized structure on any failure
code_and_size load_code(const char* name_of_file);

#endif // PARSE_ASM_FROM_FILE_H