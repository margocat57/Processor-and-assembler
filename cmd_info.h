#ifndef CMD_INFO_H
#define CMD_INFO_H
#include "assembler_task/parsing_str.h"
#include "processor_task/processor.h"
#include "processor_task/do_instructions.h"

#ifdef ASSEMBLER
    #define ASSEMBLER(...) __VA_ARGS__
#else 
    #define ASSEMBLER(...)
#endif //ASSEMBLER

#ifdef PROCESSOR
    #define PROCESSOR(...) __VA_ARGS__
#else 
    #define PROCESSOR(...)
#endif //PROCESSOR

struct comands_and_size{
    const char* name_of_comand;
    size_t hash;
    size_t size;
    CODE_CMD bytecode;
    int num_of_params;
    ASSEMBLER(assembler_err_t(*function_asm)(int, assembler*);)
    PROCESSOR(stack_err_bytes(*function_proc)(processor*);)
};


const comands_and_size COMANDS[]={
    {},                                                                                          //0
    {"PUSH",  6384411237   , 4, PUSH,   1, ASSEMBLER(push,)            PROCESSOR(proc_push)},    //1
    {"ADD" ,  193450094    , 3, ADD,    0, ASSEMBLER(other,)           PROCESSOR(add)},          //2
    {"SUB" ,  193470255    , 3, SUB,    0, ASSEMBLER(other,)           PROCESSOR(sub)},          //3
    {"DIV" ,  193453544    , 3, DIV,    0, ASSEMBLER(other,)           PROCESSOR(divide)},       //4
    {"MUL" ,  193463731    , 3, MUL,    0, ASSEMBLER(other,)           PROCESSOR(mul)},          //5
    {"OUT" ,  193465917    , 3, OUT,    0, ASSEMBLER(other,)           PROCESSOR(out)},          //6
    {"HLT" ,  193457997    , 3, HLT,    0, ASSEMBLER(other,)           PROCESSOR(NULL)},         //7
    {"SQRT" , 6384514671   , 4, SQRT,   0, ASSEMBLER(other,)           PROCESSOR(sqrt)},         //8
    {"IN"   , 5862396      , 2, IN,     0, ASSEMBLER(other,)           PROCESSOR(in)},           //9
    {"JB"   , 5862417      , 2, JB,     1, ASSEMBLER(func_with_metka,) PROCESSOR(jb)},           //10
    {"JBE"  , 193459830    , 3, JBE,    1, ASSEMBLER(func_with_metka,) PROCESSOR(jbe)},          //11
    {"JA"   , 5862416      , 2, JA,     1, ASSEMBLER(func_with_metka,) PROCESSOR(ja)},           //12
    {"JAE"  , 193459797    , 3, JAE,    1, ASSEMBLER(func_with_metka,) PROCESSOR(jae)},          //13
    {"JE"   , 5862420      , 2, JE,     1, ASSEMBLER(func_with_metka,) PROCESSOR(je)},           //14
    {"JNE"  , 193460226    , 3, JNE,    1, ASSEMBLER(func_with_metka,) PROCESSOR(jne)},          //15
    {"JMP"  , 193460204    , 3, JMP,    1, ASSEMBLER(func_with_metka,) PROCESSOR(jmp)},          //16
    {"CALL" , 6383922049   , 4, CALL,   1, ASSEMBLER(func_with_metka,) PROCESSOR(call)},         //17
    {"RET"  , 193468656    , 3, RET,    0, ASSEMBLER(other,)           PROCESSOR(ret)},          //18
    {"PUSHM", 210685570898 , 5, PUSHM,  1, ASSEMBLER(pushm_popm,)      PROCESSOR(pushm)},        //19
    {"POPM",  6384404609   , 4, POPM,   1, ASSEMBLER(pushm_popm,)      PROCESSOR(popm)},         //20
    {"DRAW",  6383976147   , 4, DRAW,   0, ASSEMBLER(other,)           PROCESSOR(draw)},         //21
    {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {},                                                  //22 - 32
    {"PUSHR", 210685570903 , 5, PUSHR,  1, ASSEMBLER(pushr_popr,)      PROCESSOR(pushr)},        //33
    {}, {}, {}, {}, {}, {}, {}, {},                                                              //34 - 41
    {"POPR",  6384404614   , 4, POPR,   1, ASSEMBLER(pushr_popr,)      PROCESSOR(popr)},         //42
};
#endif //CMD_INFO_H