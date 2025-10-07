#include <stdio.h>
#include <math.h>
#include "parse_asm_from_file.h"
#include "assembler_task/parsing_str.h"
#include "calcul.h"
#include "stack_for_calcul/stack_func.h"
#include "stack_for_calcul/my_assert.h"
#include "stack_for_calcul/stack.h"
#include "parse_asm_from_file.h"

static void add(processor* intel);

static void mul(processor* intel);

static void div(processor* intel);

static void sub(processor* intel);

static void sqrt(processor* intel);

res_and_err calculate(processor* intel){
    res_and_err res = {};
    res.proc_err = processor_verify(intel);
    if(res.proc_err){
        return res;
    }

    int result = 0;
    int temp = 0;
    char c = 'o'; // for pause debug
    processor_dump(intel);

    for(; intel->ic < intel->code.size;){
        switch (intel->code.comands[intel->ic])
        {
        fprintf(stderr, "intel->code.comands[intel->ic] = %d", intel->code.comands[intel->ic]);
        case PUSH:
            stack_push(intel->stack, &(intel->code.comands[intel->ic + 1]));
            intel->ic++;
            // FIXME пахнет говнокодом убрать
            intel->ic++;
            // printf("PUSH\n");
            // stack_dump(intel->stack);
            break;
        case ADD:
            // printf("before ADD\n");
            // stack_dump(intel->stack);
            add(intel);
            intel->ic++;
            // printf("after ADD\n");
            // stack_dump(intel->stack);
            break;
        case SUB:
            // printf("bef SUB\n");
            // stack_dump(intel->stack);
            sub(intel);
            intel->ic++;
            // printf("after SUB\n");
            // stack_dump(intel->stack);
            break;
        case DIV:
            // printf("bef DIV\n");
            // stack_dump(intel->stack);
            div(intel);
            intel->ic++;
            // printf("after DIV\n");
            // stack_dump(intel->stack);
            break;
        case MUL:
            // printf("bef mul\n");
            // stack_dump(intel->stack);
            mul(intel);
            intel->ic++;
            // printf("aft mul\n");
            // stack_dump(intel->stack);
            break;
        case SQRT:
            sqrt(intel);
            intel->ic++;
            // printf("SQRT\n");
            // stack_dump(intel->stack);
            break;
        case OUT:
            stack_pop(intel->stack, &result);
            res.res = result;
            printf("res = %d\n", res.res);
            intel->ic++;
            break;
        case VLT:
            res.proc_err = processor_verify(intel);
            intel->ic++;
            return res;
        case IN:
            scanf("%d", &temp);
            stack_push(intel->stack, &temp);
            intel->ic++;
            break;
        case POPR:
            intel->ic++;
            // printf("POPR\n");
            stack_pop(intel->stack, &temp);
            // stack_dump(intel->stack);
            intel->registr[(intel->code.comands)[intel->ic]] = temp;
            intel->ic++;
            break;
        case PUSHR:
            intel->ic++;
            // printf("PUSHR\n");
            temp = intel->registr[(intel->code.comands)[intel->ic]];
            stack_push(intel->stack, &temp);
            intel->ic++;
            // stack_dump(intel->stack);
            break;
        case JMP:
            intel -> ic = intel->code.comands[intel -> ic + 1];
            printf("Введите символ чтобы продолжить\n");
            c = getchar();
            break;
        default:
            printf("intel -> ic = %d", intel -> ic);
            printf("intel->code.comands[intel -> ic] = %d", intel->code.comands[intel -> ic]);
            fprintf(stderr, "INCORRECT CMD CODE");
            return res;
        }
    }
    res.proc_err = processor_verify(intel);
    return res;
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
    int pop = 0;
    double temp = 0;

    stack_pop(intel->stack, &pop);

    if(pop >= 0){
        temp = sqrt(pop);
        pop = round(temp);
        stack_push(intel->stack, &pop);
    }
}
