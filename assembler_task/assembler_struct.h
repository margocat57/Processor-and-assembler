//! @file
#ifndef ASSEMBLER_STRUCT
#define ASSEMBLER_STRUCT
#include <string.h>
#include "metki.h"

typedef int64_t assembler_err_t;
struct file_in_array{
    size_t amount_str;          //!amount of strings in file       
    char *all_strings_in_file;  //!array with all strings
    bool is_stat_err;           //!if there is miskate ar stat's working
};

struct instruction_info{
    long long int pc;           
    char* instruction; //TODO to const char
    size_t hash;
    int bytecode;
    int num_of_args;
    int args;
};

struct assembler{
    struct file_in_array file_in_arr; //! struct containing number of strings, array with all strings and info is there was mistake at stat's working
    char** ptr_array;                 //! array with pointers to all strings
    int* bytecode;                    //! bytecode array
    struct metki metki_asm;           //! metki array
    size_t asm_bytecode_size;         //! bytecode size
    size_t asm_pc;                    //! pointer to instruction that is assembling
    struct instruction_info* info;    //! struct for more convenient work and asm dump: constaining pointer to command, instruction, command bytecode, num of args of command, args
};

enum assembler_err{
    NO_MISTAKE_ASM          = 0,
    ALLOC_ERROR_ASM         = 1 << 0,
    INCORRECT_METKA         = 1 << 1,
    INCORRECT_REGISTR       = 1 << 2,
    INCORRECT_CMD           = 1 << 3,
    NULL_PTR                = 1 << 4,
};

//! @brief Initializes and populates assembler structure with parsed data
//!
//! This function performsinitialization of the assembler structure
//! by reading and processing the input assembly file through multiple stages.
//! If any stage fails, all allocated resources are cleaned up and an empty
//! structure is returned.
//!
//! @par Initialization Sequence:
//! 1. Read file content into string array (file_in_arr)
//! 2. Create pointer array for line processing (ptr_array) 
//! 3. Fill listing structure with instructions and bytecode indices (info)
//! 4. Initialize labels/metki array for jump targets (metki_asm)
//! 5. Parse instructions into final bytecode (parser)
//! 6. Dump struct, that was inicialized (asm_dump)
assembler asm_init(const char* instructions_file);

void asm_dump(assembler* assembl);

//! @brief Safely deallocates all dynamic memory in assembler structure
//!
//! This function performs comprehensive cleanup of the assembler structure by:
//! 1. Checking if each pointer field is non-NULL
//! 2. Zeroing out the memory content for security
//! 3. Freeing the allocated memory
//! 4. Setting pointers to NULL to prevent use-after-free
//!
//! Cleanup Sequence:
//! 1. file_in_arr.all_strings_in_file - String array from file reading
//! 2. bytecode - array with bytecode of all instructions
//! 3. ptr_array - Array of string pointers for lines in string array
//! 4. info - Listing structure with instructions and indices
//! 5. metki_asm - Labels array (via metki_destroy)
void free_asm(assembler* assembl);

#endif //ASSEMBLER_STRUCT