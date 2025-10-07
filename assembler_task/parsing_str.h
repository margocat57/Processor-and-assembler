#ifndef PARSING_STR_H
#define PARSING_STR_H
#include <stdlib.h>

const char* const BYTECODE_AUTOR_STR = "Bytecode_BBM_V2\n";

const int AMNT_CMD = 17;

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
    JB   = 10,
    JBE  = 11,
    JA   = 12,
    JAE  = 13,
    JE   = 14,
    JNE  = 15,
    POPR = 42,
    PUSHR = 33,
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
    {"PUSH", 4, PUSH}, //0
    {"ADD",  3, ADD},  //1
    {"SUB",  3, SUB},  //2
    {"DIV",  3, DIV}, //3
    {"MUL",  3, MUL},//4
    {"OUT",  3, OUT},//5
    {"VLT",  3, VLT},//6
    {"SQRT", 4, SQRT},//7
    {"IN",   2, IN},//8
    {"POPR", 4, POPR},//9
    {"PUSHR",5, PUSHR},//10
    {"JB"   , 2,  JB},//11
    {"JBE"  , 3, JBE},//12
    {"JA"   , 2,  JA},//13
    {"JAE"  , 3, JAE},//14
    {"JE"   , 2,  JE},//15
    {"JNE"  , 3, JNE}//16
};

void parse_comands(char** ptr_arr, size_t num_of_str, const char* name_of_file);
// bytecode parse_comands(char** ptr_arr, size_t num_of_str);

#endif //PARSING_STR_H