//!@file
#ifndef PARSING_STR_H
#define PARSING_STR_H
#include <stdlib.h>
#include "assembler_struct.h"

const char* const BYTECODE_AUTOR_STR = "Bytecode_BBM_V2";

const int ELEM_IN_STR = 15;

const int AMNT_CMD = 43;

enum CODE_CMD{
    PUSH  = 1,
    ADD   = 2,
    SUB   = 3,
    DIV   = 4,
    MUL   = 5,
    OUT   = 6,
    VLT   = 7,
    SQRT  = 8,
    IN    = 9,
    JB    = 10,
    JBE   = 11,
    JA    = 12,
    JAE   = 13,
    JE    = 14,
    JNE   = 15,
    JMP   = 16,
    CALL  = 17,
    RET   = 18,
    PUSHM = 19,
    POPM  = 20,
    DRAW  = 21,
    PUSHR = 33,
    POPR  = 42,
};

//! needed to know which function needed to be called to parse this parameter
enum TYPE_OF_ELEM{
    PUSHRM_POPRM,   //! call function to parse pushr/popr/pushm/popm commands
    JUMP_WITH_COND, //! call function to parse jump or call commands
    PUSH_TYPE,      //! call function to parse push commands
    OTHER           //! call function to parse other commands
};

struct comands_and_size{
    const char* name_of_comand;
    size_t size;
    CODE_CMD bytecode;
    int num_of_params;
    TYPE_OF_ELEM elem_type;
};

const comands_and_size COMANDS[]={
    {},                                         //0
    {"PUSH", 4,  PUSH,   1, PUSH_TYPE},         //1
    {"ADD" ,  3, ADD,    0, OTHER},             //2
    {"SUB" ,  3, SUB,    0, OTHER},             //3
    {"DIV" ,  3, DIV,    0, OTHER},             //4
    {"MUL" ,  3, MUL,    0, OTHER},             //5
    {"OUT" ,  3, OUT,    0, OTHER},             //6
    {"VLT" ,  3, VLT,    0, OTHER},             //7
    {"SQRT" , 4, SQRT,   0, OTHER},             //8
    {"IN"   , 2, IN,     0, OTHER},             //9
    {"JB"   , 2, JB,     1, JUMP_WITH_COND},    //10
    {"JBE"  , 3, JBE,    1, JUMP_WITH_COND},    //11
    {"JA"   , 2, JA,     1, JUMP_WITH_COND},    //12
    {"JAE"  , 3, JAE,    1, JUMP_WITH_COND},    //13
    {"JE"   , 2, JE,     1, JUMP_WITH_COND},    //14
    {"JNE"  , 3, JNE,    1, JUMP_WITH_COND},    //15
    {"JMP"  , 3, JMP,    1, JUMP_WITH_COND},    //16
    {"CALL" , 4, CALL,   1, JUMP_WITH_COND},    //17
    {"RET"  , 3, RET,    0, OTHER},             //18
    {"PUSHM", 5, PUSHM,  1, PUSHRM_POPRM},      //19
    {"POPM",  4, POPM,  1,  PUSHRM_POPRM},      //20
    {"DRAW",  4, DRAW,  0,  OTHER},             //21
    {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, //22 - 32
    {"PUSHR", 5, PUSHR, 1,  PUSHRM_POPRM},      //33
    {}, {}, {}, {}, {}, {}, {}, {},             //34 - 41
    {"POPR",  4, POPR,  1,  PUSHRM_POPRM},      //42
};

//! @brief Second pass: Generates actual bytecode from parsed instructions and labels
//!
//! Operation:
//! 1. Validates assembler pointer is not NULL
//! 2. Verifies labels array (metki) is properly initialized
//! 3. Allocates zero-initialized bytecode array based on calculated size
//! 4. Iterates through all instructions using asm_pc counter
//! 5. Skips empty instructions (NULL instruction pointers)
//! 6. Processes each non-empty command through parse_cmnds() function
//! 7. Stops and returns immediately on any parsing error
//!
//! @note Uses asm_pc counter to track through instruction array
//!
//! @warning Bytecode array allocation size comes from first pass calculation
//! @warning All metki must be inicialized before this stage 
assembler_err_t parser(assembler* assembl);

//! @brief Fills listing structure with instruction metadata and PC values
//!
//! This function performs the initial parsing pass through the assembly code,
//! identifying instructions, metki, and calculating their program counter (PC) positions
//! in the bytecode array.
//!
//! PC Logic:
//! - Regular commands: PC = current bytecode size, increase by 1 for next cmd
//! - Commands with parameters: PC = current bytecode size, increase by 2 for next cmd
//! - Metki: PC = -1 (special marker for metki's definitions, need for metki array)
//!
//! - Skips leading whitespace before comparison
//! - Uses strncmp with command-specific size for precise matching
//! - Handles both parameterized and non-parameterized commands
//! - Calls put_params() to store instruction details in listing
//!
//! @note This is the first parsing pass - determines PC positions before actual code generation
//! @warning Returns NULL if memory allocation fails or input validation fails
listing* fill_listing_struct(assembler* assembl);

#endif //PARSING_STR_H