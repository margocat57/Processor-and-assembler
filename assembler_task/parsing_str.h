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
    HLT   = 7,
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
instruction_info* fill_listing_struct(assembler* assembl);


//! @brief Processes register-based PUSH/POP operations
//!
//! For register/ram operations:
//! - Writes command bytecode to array and listing
//! - Extracts register name from instruction 
//! - Validates register letter is in range of max number of registers
//! - Returns error if register is outside valid range
//! - Converts register letter to numeric index
//! - Writes register index to NEXT position in bytecode (pc + 1)

assembler_err_t pushr_popr(int cmd, assembler* assembl);

assembler_err_t pushm_popm(int cmd, assembler* assembl);

//! @brief Processes jump commands with label targets
//!
//! For conditional/unconditional jumps:
//! - Writes jump command bytecode to bytecode array  
//! - Stores command in listing structure
//! - Extracts label number from instruction (text after colon)
//! - Looks up the target address from  metki array
//! - Writes the resolved address to NEXT position in bytecode (pc + 1)
//! - Stores both command and target address in listing
assembler_err_t func_with_metka(int cmd, assembler* assembl);

//! @brief Processes PUSH commands with immediate numeric values
//!
//! For PUSH commands with direct values:
//! - Writes the PUSH command bytecode to bytecode array
//! - Stores the command in listing structure
//! - Extracts numeric value from instruction string (after command name + space)
//! - Writes the numeric argument to the next position in bytecode array (pc + 1)
//! - Stores both command and argument in listing array
assembler_err_t push(int cmd, assembler* assembl);

//! @brief Processes simple commands without arguments (e.g., ADD, SUB, MUL)
//! 
//! For commands that don't take any arguments:
//! - Writes the command bytecode to the main bytecode array
//! - Stores the same bytecode in the listing structure for debugging
//! - Sets num_of_args to 0 in the listing array
assembler_err_t other(int cmd, assembler* assembl);



#endif //PARSING_STR_H