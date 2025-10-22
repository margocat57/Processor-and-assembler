//!@file
#include <stdio.h>
#include <math.h>
#include "parse_asm_from_file.h"
#include "../assembler_task/parsing_str.h"
#include "do_instructions.h"
#include "../stack_for_calcul/stack_func.h"
#include "../stack_for_calcul/my_assert.h"
#include "../stack_for_calcul/stack.h"
#include "parse_asm_from_file.h"

#define CHECK_STACK_ERR(error) if (error != 0) { return error; }

static stack_err_bytes jump_with_condition(processor* intel);

static stack_err_bytes do_arithmetic_op(processor* intel);

static stack_err_bytes proc_push(processor* intel);

static stack_err_bytes sqrt(processor* intel);

static stack_err_bytes jmp(processor*intel);

static stack_err_bytes out(processor* intel);

//! in - Reads input from keyboard and pushes it in the stack 
static stack_err_bytes in(processor* intel);

static stack_err_bytes popr(processor* intel);

static stack_err_bytes pushr(processor* intel);

//! popm - Pops the top element from the stack into RAM at the index specified in the register
static stack_err_bytes popm(processor* intel);

//! pushm - Pushes an element from RAM into the stack using the index specified in the register
static stack_err_bytes pushm(processor* intel);

//! call - Saves the next instruction address after the call in the call stack and jumps to the target label
static stack_err_bytes call(processor* intel);

//! ret - Takes the address of the last instruction from the call stack and jumps to it
static stack_err_bytes ret(processor* intel);

// а здесь ругается что такой способ объявления массива устарел - хотя имхо довольно таки удобно, как исправить этот момент
static stack_err_bytes(*functions[AMNT_CMD])(processor*) = {
    [PUSH]  =  proc_push,
    [ADD]   =  do_arithmetic_op,
    [SUB]   =  do_arithmetic_op,
    [DIV]   =  do_arithmetic_op,
    [MUL]   =  do_arithmetic_op,
    [SQRT]  =  sqrt,
    [OUT]   =  out,
    [IN]    =  in,
    [POPR]  =  popr,
    [PUSHR] =  pushr,
    [JB]    =  jump_with_condition,
    [JBE]   =  jump_with_condition,
    [JA]    =  jump_with_condition,
    [JAE]   =  jump_with_condition,
    [JE]    =  jump_with_condition,
    [JNE]   =  jump_with_condition,
    [JMP]   =  jmp,
    [CALL]  =  call,
    [RET]   =  ret,
    [PUSHM] =  pushm,
    [POPM]  =  popm,
    [DRAW]  =  ram_dump,
};

stack_err_bytes do_processor_comands(processor* intel){
    stack_err_bytes res = NO_MISTAKE;
    res = processor_verify(intel);
    if(res){
        return res;
    }

    int bytecode_elem = 0;
    // char ch = 'o'; // for pause debug
    // processor_dump(intel);

    for(; intel->ic < intel->code.size;){
        bytecode_elem = intel->code.comands[intel->ic];
        if(bytecode_elem == VLT){
            res = processor_verify(intel);
            return res;
        }
        if (bytecode_elem >= AMNT_CMD || !functions[bytecode_elem]){
            fprintf(stderr, "INCORRECT COMAND");
            return INCORR_COMAND;
        }
        res = functions[bytecode_elem](intel);
        if(res){
            return res;
        }
        // begin DEBUG code
        // printf("after:\n");
        // processor_dump(intel);
        // printf("Enter char to continue\n");
        // ch = getchar();
        // end DEBUG code
    }
    res = processor_verify(intel);
    return res;
}

static stack_err_bytes proc_push(processor* intel){
    CHECK_STACK_ERR(stack_push(intel->stack, &(intel->code.comands[intel->ic + 1])));
    intel->ic++;
    intel->ic++;
    return NO_MISTAKE;
}


static stack_err_bytes popr(processor* intel){
    int temp = 0;
    intel->ic++;
    CHECK_STACK_ERR(stack_pop(intel->stack, &temp));  
    intel->registr[(intel->code.comands)[intel->ic]] = temp;
    intel->ic++;
    return NO_MISTAKE;
}

static stack_err_bytes pushr(processor* intel){
    int temp = 0;
    intel->ic++;
    temp = intel->registr[(intel->code.comands)[intel->ic]];
    CHECK_STACK_ERR(stack_push(intel->stack, &temp));
    intel->ic++;
    return NO_MISTAKE;
}

static stack_err_bytes popm(processor* intel){
    int temp = 0;
    intel->ic++;

    intel->ram_counter = intel->registr[(intel->code.comands)[intel->ic]];
    if(intel->ram_counter >= RAM_MAX_SIZE){
        fprintf(stderr, "RAM is full - can't add elem to ram");
        intel->ic++;
        return RAM_OVERFLOW;
    }

    CHECK_STACK_ERR(stack_pop(intel->stack, &temp));
    intel->RAM[intel->ram_counter] = temp;

    intel->ic++;
    return NO_MISTAKE;
}

static stack_err_bytes pushm(processor* intel){
    int temp = 0;

    intel->ic++;
    intel->ram_counter = intel->registr[(intel->code.comands)[intel->ic]];
    temp = intel->RAM[intel->ram_counter];

    CHECK_STACK_ERR(stack_push(intel->stack, &temp));
    intel->ic++;

    return NO_MISTAKE;
}

static stack_err_bytes in(processor* intel){
    int temp = 0;
    printf("Input parametr:\n");
    scanf("%d", &temp);
    CHECK_STACK_ERR(stack_push(intel->stack, &temp));
    intel->ic++;
    return NO_MISTAKE;
}

static stack_err_bytes out(processor* intel){
    CHECK_STACK_ERR(stack_pop(intel->stack, &intel->result));
    printf("result = %d\n", intel->result);
    intel->ic++;
    return NO_MISTAKE;
}

static stack_err_bytes sqrt(processor* intel){
    int pop = 0;
    double temp = 0;

    CHECK_STACK_ERR(stack_pop(intel->stack, &pop));

    if(pop >= 0){
        temp = sqrt(pop);
        pop = (int)round(temp);
        CHECK_STACK_ERR(stack_push(intel->stack, &pop));
    }

    intel->ic++;
    return NO_MISTAKE;
}

static stack_err_bytes call(processor* intel){
    intel->ic += 2; // увеличиваем счетчик на 2 чтобы добраться до команды, которая следуер за call и меткой
    int return_address = (int)intel->ic;
    CHECK_STACK_ERR(stack_push(intel->call_stack, &return_address));

    intel->ic--; //  перескакиваем на прошлую команду
    intel -> ic = (size_t)intel->code.comands[intel -> ic]; 

    return NO_MISTAKE;
}

static stack_err_bytes ret(processor* intel){
    int idx = 0;
    CHECK_STACK_ERR(stack_pop(intel->call_stack, &idx));
    intel->ic = (size_t)idx; //да будет ругаться, что из int в size_t но стек на интах и тут ничего не поделаешь

    return NO_MISTAKE;
}


static stack_err_bytes jmp(processor* intel){
    intel -> ic = (size_t)intel->code.comands[intel -> ic + 1];
    return NO_MISTAKE;
}

static stack_err_bytes do_arithmetic_op(processor* intel){
    int stack_top_el = 0; 
    CHECK_STACK_ERR(stack_pop((intel)->stack, &stack_top_el)); 

    int stack_el = 0; 
    CHECK_STACK_ERR(stack_pop((intel)->stack, &stack_el)); 

    int res = 0;
    switch(intel->code.comands[intel->ic]){
        case ADD: res = stack_el + stack_top_el; break;
        case SUB: res = stack_el - stack_top_el; break;
        case MUL: res = stack_el * stack_top_el; break;
        case DIV: if (stack_top_el == 0){
            fprintf(stderr, "ZERO DIVISION - INCORRECT");
            return ZERO_DIV;
        }
        res = stack_el / stack_top_el; break;
        default: return INCORR_COMAND;
    }

    CHECK_STACK_ERR(stack_push((intel)->stack, &res));
    intel->ic++; 
    return NO_MISTAKE;
}

static stack_err_bytes jump_with_condition(processor* intel){
    int t1 = 0, t2 = 0; 
    CHECK_STACK_ERR(stack_pop((intel)->stack, &t1)); 
    CHECK_STACK_ERR(stack_pop((intel)->stack, &t2)); 

    bool condition = false;

    switch(intel->code.comands[intel->ic]){
        case JA:  condition = t1 > t2;  break;
        case JAE: condition = t1 >= t2; break;
        case JB:  condition = t1 < t2;  break;
        case JBE: condition = t1 <= t2; break;
        case JE:  condition = t1 == t2; break;
        case JNE: condition = t1 != t2; break;
        default: return INCORR_COMAND;
    }

    if(condition){
        intel -> ic = (size_t)intel->code.comands[intel -> ic + 1];
        return NO_MISTAKE;
    }
    intel->ic++; // перепрыгиваем на следуюбщий элемент - номер строки
    intel->ic++; // перепрыгиваем на следующую команду
    return NO_MISTAKE;
}