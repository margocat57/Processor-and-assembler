#include <stdio.h>
#include <math.h>
#include "parse_asm_from_file.h"
#include "assembler_task/parsing_str.h"
#include "calcul.h"
#include "stack_for_calcul/stack_func.h"
#include "stack_for_calcul/stack.h"
#include "parse_asm_from_file.h"

static void add(processor* intel);

static void mul(processor* intel);

static void div(processor* intel);

static void sub(processor* intel);

static void sqrt(processor* intel);

int calculate(processor* intel){
    int result = 0;
    int temp = 0;
    processor_dump(intel);

    for(; intel->ic < intel->code.size; intel->ic++){
        switch (intel->code.comands[intel->ic])
        {
        case PUSH:
            stack_push(intel->stack, &(intel->code.comands[intel->ic + 1]));
            intel->ic++;
            // printf("PUSH\n");
            // stack_dump(intel->stack);
            break;
        case ADD:
            // printf("before ADD\n");
            // stack_dump(intel->stack);
            add(intel);
            // printf("after ADD\n");
            // stack_dump(intel->stack);
            break;
        case SUB:
            sub(intel);
            break;
        case DIV:
            div(intel);
            break;
        case MUL:
            mul(intel);
            break;
        case SQRT:
            sqrt(intel);
            break;
        case OUT:
            stack_pop(intel->stack, &result);
            break;
        case VLT:
            return result;
        case IN:
            scanf("%d", &temp);
            stack_push(intel->stack, &temp);
            break;
        case POPR:
            intel->ic++;
            // printf("POPR\n");
            stack_pop(intel->stack, &temp);
            // stack_dump(intel->stack);
            intel->registr[(intel->code.comands)[intel->ic]] = temp;
            break;
        case PUSHR:
            intel->ic++;
            // printf("PUSHR\n");
            temp = intel->registr[(intel->code.comands)[intel->ic]];
            stack_push(intel->stack, &temp);
            // stack_dump(intel->stack);
            break;
        default:
            fprintf(stderr, "INCORRECT CMD CODE");
            return result;
        }
    }
    return result;
}

static void add(processor* intel){
    int pop = 0;
    int temp = 0;
    for(int idx = 0; idx < 2; idx ++){
        stack_pop(intel->stack, &pop);
        temp += pop;
    }
    stack_push(intel->stack, &temp);
}

static void mul(processor* intel){
    int pop = 0;
    int temp = 1;
    for(int idx = 0; idx < 2; idx ++){
        stack_pop(intel->stack, &pop);
        temp *= pop;
    }
    stack_push(intel->stack, &temp);
}

static void sub(processor* intel){
    int pop = 0;
    int temp = 0;

    stack_pop(intel->stack, &pop);
    temp -= pop;

    stack_pop(intel->stack, &pop);
    temp += pop;

    stack_push(intel->stack, &temp);
}

static void div(processor* intel){
    int pop = 0;
    double temp = 1;

    stack_pop(intel->stack, &pop);
    temp /= (double)pop;

    stack_pop(intel->stack, &pop);
    temp *= pop;

    pop = temp;
    stack_push(intel->stack, &pop);
}

static void sqrt(processor* intel){
    double a = 0;
    double b = 0;
    double c = 0;
    int pop = 0;
    double temp = 0;

    stack_pop(intel->stack, &pop);
    c = pop;

    stack_pop(intel->stack, &pop);
    b = pop;

    stack_pop(intel->stack, &pop);
    a = pop;

    temp = b * b - 4 * a * c;
    if(temp >= 0){
        temp = sqrt(temp);
        pop = round(temp);
        stack_push(intel->stack, &pop);
    }
}
