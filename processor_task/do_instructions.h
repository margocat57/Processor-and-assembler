#ifndef CALCUL_H
#define CALCUL_H
#include "../stack_for_calcul/stack_func.h"
#include "../assembler_task/parsing_str.h"
#include "../stack_for_calcul/stack.h"
#include "processor.h"
#include "parse_asm_from_file.h"

#define CHECK_STACK_ERR(error) if (error != 0) { return error; }

#define create_func_arithm_operations(operation, name) \
    stack_err_bytes name(processor* intel) { \
        int stack_top_el = 0; \
        CHECK_STACK_ERR(stack_pop((intel)->stack, &stack_top_el)); \
        \
        int stack_el = 0;  \
        CHECK_STACK_ERR(stack_pop((intel)->stack, &stack_el)); \
        \
        int res = stack_el operation stack_top_el;    \
        CHECK_STACK_ERR(stack_push((intel)->stack, &res)); \
        intel->ic++; \
        return NO_MISTAKE;\
    }

create_func_arithm_operations(+, add);
create_func_arithm_operations(-, sub);
create_func_arithm_operations(*, mul);
create_func_arithm_operations(/, div);

#define create_jmp_cond_func(cond, name_jmp) \
    stack_err_bytes name_jmp(processor* intel) { \
        int t1 = 0, t2 = 0; \
        CHECK_STACK_ERR(stack_pop((intel)->stack, &t1)); \
        CHECK_STACK_ERR(stack_pop((intel)->stack, &t2)); \
        \
        bool condition = t1 cond t2; \
        if(condition){ \
            intel -> ic = (size_t)intel->code.comands[intel -> ic + 1]; \
            return NO_MISTAKE; \
        } \
        intel->ic++;  \
        intel->ic++; \ 
        return NO_MISTAKE; \
    }


create_jmp_cond_func(>,  ja);
create_jmp_cond_func(>=, jae);
create_jmp_cond_func(<,  jb);
create_jmp_cond_func(<=, jbe);
create_jmp_cond_func(==, je);
create_jmp_cond_func(!=, jne);


//! @brief Doing processor instructions 
//!
//! Operation:
//! 1. Validates processor state via processor_verify()
//! 2. Iterates through all bytecode instructions using instruction counter (ic)
//! 3. Call the function thar executes instruction
//! 4. Returns error on unknown command codes
stack_err_bytes do_processor_comands(processor* intel);

stack_err_bytes jump_with_condition(processor* intel);

stack_err_bytes do_arithmetic_op(processor* intel);

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
