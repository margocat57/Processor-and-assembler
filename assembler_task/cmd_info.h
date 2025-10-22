#ifndef CMD_INFO_H
#define CMD_INFO_H
#include "parsing_str.h"


struct comands_and_size{
    const char* name_of_comand;
    size_t hash;
    size_t size;
    CODE_CMD bytecode;
    int num_of_params;
    assembler_err_t(*function)(int, assembler*);
};

comands_and_size COMANDS[]={
    {},                                            //0
    {"PUSH",  0, 4, PUSH,   1, push},              //1
    {"ADD" ,  0, 3, ADD,    0, other},             //2
    {"SUB" ,  0, 3, SUB,    0, other},             //3
    {"DIV" ,  0, 3, DIV,    0, other},             //4
    {"MUL" ,  0, 3, MUL,    0, other},             //5
    {"OUT" ,  0, 3, OUT,    0, other},             //6
    {"VLT" ,  0, 3, VLT,    0, other},             //7
    {"SQRT" , 0, 4, SQRT,   0, other},             //8
    {"IN"   , 0, 2, IN,     0, other},             //9
    {"JB"   , 0, 2, JB,     1, func_with_metka},   //10
    {"JBE"  , 0, 3, JBE,    1, func_with_metka},   //11
    {"JA"   , 0, 2, JA,     1, func_with_metka},   //12
    {"JAE"  , 0, 3, JAE,    1, func_with_metka},   //13
    {"JE"   , 0, 2, JE,     1, func_with_metka},   //14
    {"JNE"  , 0, 3, JNE,    1, func_with_metka},   //15
    {"JMP"  , 0, 3, JMP,    1, func_with_metka},   //16
    {"CALL" , 0, 4, CALL,   1, func_with_metka},   //17
    {"RET"  , 0, 3, RET,    0, other},             //18
    {"PUSHM", 0, 5, PUSHM,  1, pushm_popm},        //19
    {"POPM",  0, 4, POPM,   1, pushm_popm},        //20
    {"DRAW",  0, 4, DRAW,   0, other},             //21
    {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {},    //22 - 32
    {"PUSHR", 0, 5, PUSHR,  1, pushr_popr},        //33
    {}, {}, {}, {}, {}, {}, {}, {},                //34 - 41
    {"POPR",  0, 4, POPR,   1, pushr_popr},        //42
};
#endif //CMD_INFO_H