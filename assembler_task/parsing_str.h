#ifndef PARSING_STR_H
#define PARSING_STR_H
#include <stdlib.h>

enum CODE_CMD{
    PUSH = 0,
    ADD  = 1,
    SUB  = 2,
    DIV  = 3,
    MUL  = 4,
    OUT  = 5,
    VLT  = 6,
    SQRT = 7
};

struct comands_and_num_of_str{
    int* comands;
    size_t size;
};

struct comands_str{
    const char* name_of_comand;
    size_t shift_ptr;
};

/*
struct comands_in_struct{
    size_t amount_str;         
    comands_digit* comands_digit; 
};
*/


const comands_str COMANDS_FOR_STRING[]={
    {"PUSH", 2}, 
    {"ADD",  1},
    {"SUB",  1},
    {"DIV",  1},
    {"MUL",  1},
    {"OUT",  1},
    {"VLT",  0},
    {"SQRT", 1}
};

const comands_str COMANDS_SIZE[]={
    {"PUSH", 4}, 
    {"ADD",  3},
    {"SUB",  3},
    {"DIV",  3},
    {"MUL",  3},
    {"OUT",  3},
    {"VLT",  3},
    {"SQRT", 4}
};


char** parse_comands(char** ptr_arr, size_t num_of_str);

#endif //PARSING_STR_H