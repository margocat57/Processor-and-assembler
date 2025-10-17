#ifndef ASSEMBLER_STRUCT
#define ASSEMBLER_STRUCT
#include <string.h>

struct file_in_array
{
    size_t amount_str;         
    char *all_strings_in_file; 
    bool is_stat_err;          
};

struct bytecode{
    int* array;
    size_t size;
};

struct assembler{
    struct file_in_array file_in_arr;
    char** ptr_array;
    // const char* bytecode_file; - вот по хорошему они должны быть но как нормально впихнуть не знаю
    // const char* instructions_file;
    struct bytecode bytecode_struct;
};

#endif //ASSEMBLER_STRUCT