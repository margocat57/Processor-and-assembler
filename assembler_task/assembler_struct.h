#ifndef ASSEMBLER_STRUCT
#define ASSEMBLER_STRUCT
#include <string.h>
#include "metki.h"

typedef uint64_t assembler_err_t;
struct file_in_array{
    size_t amount_str;         
    char *all_strings_in_file; 
    bool is_stat_err;          
};


struct listing{
    size_t pc;
    char* instruction;
    int bytecode;
};

struct assembler{
    struct file_in_array file_in_arr;
    char** ptr_array;
    int* bytecode;
    struct metki metki_asm;
    size_t asm_bytecode_size;
    size_t asm_pc;
    struct listing* info;
};

enum assembler_err{
    NO_MISTAKE              = 0,
    ALLOC_ERROR             = 1 << 0,
    INCORRECT_METKA         = 1 << 1,
    INCORRECT_REGISTR       = 1 << 2,
    INCORRECT_CMD           = 1 << 3
    // NULL_STR_PTR            = 1 << 3,
};

assembler asm_init(const char* instructions_file);

void free_asm(assembler* assembl);

#endif //ASSEMBLER_STRUCT