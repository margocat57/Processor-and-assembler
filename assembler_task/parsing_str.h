#ifndef PARSING_STR_H
#define PARSING_STR_H
#include <stdlib.h>

const int AMNT_CMD = 11;

enum CODE_CMD{
    PUSH = 1,
    ADD  = 2,
    SUB  = 3,
    DIV  = 4,
    MUL  = 5,
    OUT  = 6,
    VLT  = 7,
    SQRT = 8,
    IN   = 9,
    POPR = 42,
    PUSHR = 33
};

struct comands_and_size{
    const char* name_of_comand;
    size_t size;
    CODE_CMD bytecode;
};

struct bytecode{
    int* array;
    int size;
};

const comands_and_size COMANDS[]={
    {"PUSH", 4, PUSH}, 
    {"ADD",  3, ADD},
    {"SUB",  3, SUB},
    {"DIV",  3, DIV},
    {"MUL",  3, MUL},
    {"OUT",  3, OUT},
    {"VLT",  3, VLT},
    {"SQRT", 4, SQRT},
    {"IN",   2, IN},
    {"POPR", 4, POPR},
    {"PUSHR",5, PUSHR}
};

void parse_comands(char** ptr_arr, size_t num_of_str, const char* name_of_file);
// bytecode parse_comands(char** ptr_arr, size_t num_of_str);

#endif //PARSING_STR_H