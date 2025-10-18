#ifndef PARSING_STR_H
#define PARSING_STR_H
#include <stdlib.h>
#include "assembler_struct.h"

const char* const BYTECODE_AUTOR_STR = "Bytecode_BBM_V2";

const int ELEM_IN_STR = 15;

const int AMNT_CMD = 43;

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
    PUSHM = 19,
    POPM = 20,
    POPR = 42,
    PUSHR = 33,
};

enum TYPE_OF_ELEM{
    PUSHRM_POPRM,
    JUMP_WITH_COND,
    PUSH_TYPE,
    OTHER
};

struct comands_and_size{
    const char* name_of_comand;
    size_t size;
    CODE_CMD bytecode;
    int num_of_params;
    TYPE_OF_ELEM elem_type;
};

// либо 20 строк кода с пустыми структурами
const comands_and_size COMANDS[]={
    {},                 //0
    {"PUSH", 4, PUSH,   1, PUSH_TYPE},  //1
    {"ADD" ,  3, ADD,   0, OTHER},   //2
    {"SUB" ,  3, SUB,   0, OTHER},   //3
    {"DIV" ,  3, DIV,   0, OTHER},   //4
    {"MUL" ,  3, MUL,   0, OTHER},   //5
    {"OUT" ,  3, OUT,   0, OTHER},   //6
    {"VLT" ,  3, VLT,   0, OTHER},   //7
    {"SQRT" , 4, SQRT,  0, OTHER},  //8
    {"IN"   , 2, IN,    0, OTHER},    //9
    {"JB"   , 2,  JB,   1, JUMP_WITH_COND},   //10
    {"JBE"  , 3, JBE,   1, JUMP_WITH_COND},   //11
    {"JA"   , 2,  JA,   1, JUMP_WITH_COND},    //12
    {"JAE"  , 3, JAE,   1, JUMP_WITH_COND},    //13
    {"JE"   , 2,  JE,   1, JUMP_WITH_COND},    //14
    {"JNE"  , 3, JNE,   1, JUMP_WITH_COND},    //15
    {"JMP"  , 3, JMP,   1, JUMP_WITH_COND},    //16
    {"CALL" , 4, CALL,  1, OTHER},   //17
    {"RET"  , 3, RET,   0, OTHER},   //18
    {"PUSHM", 5, PUSHM, 1, PUSHRM_POPRM}, //19
    {"POPM", 4, POPM,   1, PUSHRM_POPRM},  //20
    {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, //21 - 32
    {"PUSHR",5, PUSHR,  1, PUSHRM_POPRM}, //33
    {}, {}, {}, {}, {}, {}, {}, {}, //34-41
    {"POPR", 4, POPR,   1, PUSHRM_POPRM},  //42
};

// bytecode parse_comands(char** ptr_arr, size_t num_of_str);
uint64_t parser(assembler* assembl);


#endif //PARSING_STR_H