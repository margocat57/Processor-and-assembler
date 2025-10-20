#ifndef CALCUL_H
#define CALCUL_H
#include "../assembler_task/parsing_str.h"
#include "../stack_for_calcul/stack.h"
#include "parse_asm_from_file.h"

//! @brief Doing processor instructions 
//!
//! Operation:
//! 1. Validates processor state via processor_verify()
//! 2. Iterates through all bytecode instructions using instruction counter (ic)
//! 3. Call the function thar executes instruction
//! 4. Returns error on unknown command codes
stack_err_bytes do_processor_comands(processor* intel);

#endif //CALCUL_H
