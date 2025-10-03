#ifndef PARSE_ASM_FROM_FILE_H
#define PARSE_ASM_FROM_FILE_H


struct asm_code{
    int* asm_array;
    int size;
};

int number_of_tests(const char* name_of_file);

asm_code parser_asm(const char* name_of_file);

#endif // PARSE_ASM_FROM_FILE_H