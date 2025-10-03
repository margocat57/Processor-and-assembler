#include <stdio.h>
#include <math.h>
#include "parse_asm_from_file.h"
#include "assembler_task/parsing_str.h"
#include "calcul.h"
#include "stack_for_calcul/stack_func.h"
#include "stack_for_calcul/stack.h"
#include "parse_asm_from_file.h"

void add(stack_t_t* stack, int* temp_mul_add_sub, int* pop);

void mul(stack_t_t* stack, int* temp_mul_add_sub, int* pop);

void sub(stack_t_t* stack, int* temp_mul_add_sub, int* pop);

void div(stack_t_t* stack, int* temp_mul_add_sub, double temp_div, int* pop);

void sqrt(stack_t_t* stack, int* temp_mul_add_sub, double temp_div, int* pop);

int calculate(asm_code* code){
    stack_t_t* stack = stack_ctor((long long int)code->size, __FILE__, __func__, __LINE__);
    int result = 0;
    int temp_mul_add_sub = 1;
    double temp_div = 1.;
    int pop = 0;
    for(size_t cmd = 0; cmd < code->size; ){
        if(code->asm_array[cmd] == PUSH + 1){
            stack_push(stack, &(code->asm_array[cmd + 1]));
            cmd += COMANDS_FOR_STRING[PUSH].shift_ptr;
        }
        else if(code->asm_array[cmd] == ADD + 1){
            temp_mul_add_sub -= 1;
            add(stack, &temp_mul_add_sub, &pop);
            cmd += COMANDS_FOR_STRING[ADD].shift_ptr;
        }
        else if(code->asm_array[cmd] == MUL + 1){
            mul(stack, &temp_mul_add_sub, &pop);
            cmd += COMANDS_FOR_STRING[MUL].shift_ptr;
        }
        else if(code->asm_array[cmd] == SUB + 1){
            temp_mul_add_sub -= 1;
            sub(stack, &temp_mul_add_sub, &pop);
            cmd += COMANDS_FOR_STRING[SUB].shift_ptr;
        }
        else if(code->asm_array[cmd] == DIV + 1){
            div(stack, &temp_mul_add_sub, temp_div, &pop);
            cmd += COMANDS_FOR_STRING[DIV].shift_ptr;
        }
        else if(code->asm_array[cmd] == OUT + 1){
            stack_pop(stack, &result);
            cmd += COMANDS_FOR_STRING[OUT].shift_ptr;
        }
        else if(code->asm_array[cmd] == VLT + 1){
            stack_free(stack);
            break;
        }
        else if(code->asm_array[cmd] == SQRT + 1){
            sqrt(stack, &temp_mul_add_sub, temp_div, &pop);
            cmd += COMANDS_FOR_STRING[SQRT].shift_ptr;
        }
        temp_mul_add_sub = 1;
    }
    return result;
}

void add(stack_t_t* stack, int* temp_mul_add_sub, int* pop){
    for(int idx = 0; idx < 2; idx ++){
        stack_pop(stack, pop);
        *temp_mul_add_sub += *pop;
    }
    stack_push(stack, temp_mul_add_sub);
}

void mul(stack_t_t* stack, int* temp_mul_add_sub, int* pop){
    for(int idx = 0; idx < 2; idx ++){
        stack_pop(stack, pop);
        *temp_mul_add_sub *= *pop;
    }
    stack_push(stack, temp_mul_add_sub);
}

void sub(stack_t_t* stack, int* temp_mul_add_sub, int* pop){
    stack_pop(stack, pop);
    *temp_mul_add_sub -= *pop;

    stack_pop(stack, pop);
    *temp_mul_add_sub += *pop;

    stack_push(stack, temp_mul_add_sub);
}

void div(stack_t_t* stack, int* temp_mul_add_sub, double temp_div, int* pop){
    stack_pop(stack, pop);
    temp_div /= (double)(*pop);

    stack_pop(stack, pop);
    temp_div *= *pop;

    *temp_mul_add_sub = temp_div;
    stack_push(stack, temp_mul_add_sub);
}

void sqrt(stack_t_t* stack, int* temp_mul_add_sub, double temp_div, int* pop){
    double a = 0;
    double b = 0;
    double c = 0;

    stack_pop(stack, pop);
    c = *pop;

    stack_pop(stack, pop);
    b = *pop;

    stack_pop(stack, pop);
    a = *pop;

    temp_div = b * b - 4 * a * c;
    if(temp_div >= 0){
        temp_div = sqrt(temp_div);
        *temp_mul_add_sub = round(temp_div);
        stack_push(stack, temp_mul_add_sub);
    }
}