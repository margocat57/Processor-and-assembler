#ifndef CALCUL_H
#define CALCUL_H
#include "../stack_for_calcul/stack_func.h"
#include "../assembler_task/parsing_str.h"
#include "../stack_for_calcul/stack.h"
#include "processor.h"
#include "parse_asm_from_file.h"

#define CHECK_STACK_ERR(error) if (error != 0) { return error; }

stack_err_bytes add(processor* intel);
stack_err_bytes sub(processor* intel);
stack_err_bytes divide(processor* intel);
stack_err_bytes mul(processor* intel);
stack_err_bytes ja(processor* intel);
stack_err_bytes jae(processor* intel);
stack_err_bytes jb(processor* intel);
stack_err_bytes jbe(processor* intel);
stack_err_bytes je(processor* intel);
stack_err_bytes jne(processor* intel);




//! @brief Doing processor instructions 
//!
//! Operation:
//! 1. Validates processor state via processor_verify()
//! 2. Iterates through all bytecode instructions using instruction counter (ic)
//! 3. Call the function thar executes instruction
//! 4. Returns error on unknown command codes
stack_err_bytes do_processor_comands(processor* intel);

stack_err_bytes draw(processor* intel);

stack_err_bytes proc_push(processor* intel);

stack_err_bytes sqrt(processor* intel);

stack_err_bytes jmp(processor*intel);

stack_err_bytes out(processor* intel);

//! in - Reads input from keyboard and pushes it in the stack 
stack_err_bytes in(processor* intel);

stack_err_bytes popr(processor* intel);

stack_err_bytes pushr(processor* intel);

//! popm - Pops the top element from the stack into RAM at the index specified in the register
stack_err_bytes popm(processor* intel);

//! pushm - Pushes an element from RAM into the stack using the index specified in the register
stack_err_bytes pushm(processor* intel);

//! call - Saves the next instruction address after the call in the call stack and jumps to the target label
stack_err_bytes call(processor* intel);

//! ret - Takes the address of the last instruction from the call stack and jumps to it
stack_err_bytes ret(processor* intel);

#endif //CALCUL_H
