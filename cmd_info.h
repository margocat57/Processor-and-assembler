#ifndef CMD_INFO_H
#define CMD_INFO_H
#include "assembler_task/parsing_str.h"
#include "processor_task/processor.h"
#include "processor_task/do_instructions.h"


struct comands_and_size{
    const char* name_of_comand;
    size_t hash;
    size_t size;
    CODE_CMD bytecode;
    int num_of_params;
    assembler_err_t(*function_asm)(int, assembler*);
    stack_err_bytes(*function_proc)(processor*);
};

//TODO у DRAW немного другая логика
const comands_and_size COMANDS[]={
    {},                                                                 //0
    {"PUSH",  6384411237    , 4, PUSH,   1, push,            proc_push}, //1
    {"ADD" ,  193450094    , 3, ADD,    0, other,           add},       //2
    {"SUB" ,  193470255    , 3, SUB,    0, other,           sub},       //3
    {"DIV" ,  193453544    , 3, DIV,    0, other,           divide},    //4
    {"MUL" ,  193463731    , 3, MUL,    0, other,           mul},       //5
    {"OUT" ,  193465917    , 3, OUT,    0, other,           out},       //6
    {"VLT" ,  193473243    , 3, VLT,    0, other,           NULL},      //7
    {"SQRT" , 6384514671   , 4, SQRT,   0, other,           sqrt},      //8
    {"IN"   , 5862396      , 2, IN,     0, other,           in},        //9
    {"JB"   , 5862417      , 2, JB,     1, func_with_metka, jb},        //10
    {"JBE"  , 193459830    , 3, JBE,    1, func_with_metka, jbe},       //11
    {"JA"   , 5862416      , 2, JA,     1, func_with_metka, ja},        //12
    {"JAE"  , 193459797    , 3, JAE,    1, func_with_metka, jae},       //13
    {"JE"   , 5862420      , 2, JE,     1, func_with_metka, je},        //14
    {"JNE"  , 193460226    , 3, JNE,    1, func_with_metka, jne},       //15
    {"JMP"  , 193460204    , 3, JMP,    1, func_with_metka, jmp},       //16
    {"CALL" , 6383922049   , 4, CALL,   1, func_with_metka, call},      //17
    {"RET"  , 193468656    , 3, RET,    0, other,           ret},       //18
    {"PUSHM", 210685570898 , 5, PUSHM,  1, pushm_popm,      pushm},     //19
    {"POPM",  193466804    , 4, POPM,   1, pushm_popm,      popm},      //20
    {"DRAW",  2089008851   , 4, DRAW,   0, other,           ram_dump},  //21
    {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {},                         //22 - 32
    {"PUSHR", 210685570903 , 5, PUSHR,  1, pushr_popr,      pushr},     //33
    {}, {}, {}, {}, {}, {}, {}, {},                                     //34 - 41
    {"POPR",  6384404614   , 4, POPR,   1, pushr_popr,      popr},      //42
};
#endif //CMD_INFO_H