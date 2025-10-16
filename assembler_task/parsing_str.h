#ifndef PARSING_STR_H
#define PARSING_STR_H
#include <stdlib.h>

// структура ассемблера - что добавить

const char* const BYTECODE_AUTOR_STR = "Bytecode_BBM_V2";

const int ELEM_IN_STR = 15;

const int AMNT_CMD = 21;

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
    JMP  = 16,
    CALL = 17,
    RET  = 18,
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
    size_t size;
};

const comands_and_size COMANDS[]={
    {},                //0
    {"PUSH", 4, PUSH}, //1
    {"ADD",  3, ADD},  //2
    {"SUB",  3, SUB},  //3
    {"DIV",  3, DIV},  //4
    {"MUL",  3, MUL},  //5
    {"OUT",  3, OUT},  //6
    {"VLT",  3, VLT},  //7
    {"SQRT", 4, SQRT}, //8
    {"IN",   2, IN},   //9
    {"JB"   , 2,  JB}, //10
    {"JBE"  , 3, JBE}, //11
    {"JA"   , 2,  JA}, //12
    {"JAE"  , 3, JAE}, //13
    {"JE"   , 2,  JE}, //14
    {"JNE"  , 3, JNE}, //15
    {"JMP"  , 3, JMP}, //16
    {"CALL" , 4, CALL}, //17
    {"RET"  , 3, RET}, //18
    {"POPR", 4, POPR}, //19
    {"PUSHR",5, PUSHR},//20
};

// bytecode parse_comands(char** ptr_arr, size_t num_of_str);
bytecode parser(char** ptr_arr, size_t num_of_str);

#endif //PARSING_STR_H