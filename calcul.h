#ifndef CALCUL_H
#define CALCUL_H
#include "assembler_task/parsing_str.h"
#include "stack_for_calcul/stack.h"
#include "parse_asm_from_file.h"

/*
#define ADD_MUL_START_VALUE(code) (code) == (ADD) ? 0 : 1
#define ADD_MUL_START_WHAT_TO_DO(code, temp, pop) (code) == (ADD) ? temp += pop : temp *= pop

int do_add_mul_eq(stack_t_t* stack, int temp, int pop, size_t max_value, CODE_CMD code);
int add_mul(int* comands, stack_t_t* stack, int stack_cnt, size_t cmd, CODE_CMD code);

#define SUB_DIV_START_VALUE(code) (code) == (SUB) ? 0 : 1
#define SUB_DIV_START_WHAT_TO_DO(code, temp, pop) (code) == (SUB) ? temp -= pop : temp /= (double) pop
#define SUB_DIV_START_WHAT_AT_LAST(code, temp, pop) (code) == (SUB) ? temp += pop : temp *= pop

double do_sub_div_eq(stack_t_t* stack, int temp, int pop, size_t max_value, CODE_CMD code);
int sub_div(int* comands, stack_t_t* stack, int stack_cnt, size_t cmd, CODE_CMD code);
*/

int calculate(asm_code* code);


#endif //CALCUL_H
