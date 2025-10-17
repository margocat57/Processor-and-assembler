#include <stdio.h>
#include <math.h>
#include "parse_asm_from_file.h"
#include "assembler_task/parsing_str.h"
#include "calcul.h"
#include "stack_for_calcul/stack_func.h"
#include "stack_for_calcul/my_assert.h"
#include "stack_for_calcul/stack.h"
#include "parse_asm_from_file.h"

#define CHECK_STACK_ERR(error) if (error != 0) { return error; } 

#define JUMP_IF(intel, comp) \
    do { \
        int t1 = 0, t2 = 0; \
        CHECK_STACK_ERR(stack_pop((intel)->stack, &t1)); \
        CHECK_STACK_ERR(stack_pop((intel)->stack, &t2)); \
        jump_if_condition_sw(intel, t1 comp t2); \
    } while(0);

#define LESS    <
#define LESS_EQ <=
#define MORE    >
#define MORE_EQ >=
#define EQ      ==
#define NOT_EQ  !=

#define DO_CASE(function) \
    function; \
    break; \

static stack_err_bytes proc_push(processor* intel);

static stack_err_bytes add(processor* intel);

static stack_err_bytes mul(processor* intel);

static stack_err_bytes div(processor* intel);

static stack_err_bytes sub(processor* intel);

static stack_err_bytes sqrt(processor* intel);

static stack_err_bytes jump_if_condition_sw(processor*intel, bool condition);

static stack_err_bytes out(processor* intel, int* result);

static stack_err_bytes in(processor* intel);

static stack_err_bytes popr(processor* intel);

static stack_err_bytes pushr(processor* intel);

static stack_err_bytes popm(processor* intel);

static stack_err_bytes pushm(processor* intel);

static stack_err_bytes call(processor* intel);

static stack_err_bytes ret(processor* intel);

stack_err_bytes do_processor_comands(processor* intel){
    stack_err_bytes res = NO_MISTAKE;
    res = processor_verify(intel);
    if(res){
        return res;
    }

    int result = 0;
    char c = 'o'; // for pause debug
    // processor_dump(intel);

    for(; intel->ic < intel->code.size;){
        switch (intel->code.comands[intel->ic])
        {
        case PUSH:  DO_CASE(proc_push(intel))
        case ADD:   DO_CASE(add(intel))
        case SUB:   DO_CASE(sub(intel)) 
        case DIV:   DO_CASE(div(intel))
        case MUL:   DO_CASE(mul(intel))
        case SQRT:  DO_CASE(sqrt(intel))
        case OUT:   DO_CASE(out(intel, &result))
        case IN:    DO_CASE(in(intel)) 
        case POPR:  DO_CASE(popr(intel))
        case PUSHR: DO_CASE(pushr(intel))
        case JB:    DO_CASE(JUMP_IF(intel, LESS))
        case JBE:   DO_CASE(JUMP_IF(intel, LESS_EQ))
        case JA:    DO_CASE(JUMP_IF(intel, MORE)) 
        case JAE:   DO_CASE(JUMP_IF(intel, MORE_EQ)) 
        case JE:    DO_CASE(JUMP_IF(intel, EQ))
        case JNE:   DO_CASE(JUMP_IF(intel, NOT_EQ))
        case JMP:   DO_CASE(jump_if_condition_sw(intel, 1))
        case CALL:  DO_CASE(call(intel)) 
        case RET:   DO_CASE(ret(intel))
        case PUSHM: DO_CASE(pushm(intel))
        case POPM:  DO_CASE(popm(intel))
        case VLT:   return processor_verify(intel);
        default:
            fprintf(stderr, "INCORRECT CMD CODE");
            res = res | INCORR_COMAND;
            return res;
        }
        // begin DEBUG code
        // printf("after:\n");
        // processor_dump(intel);
        // printf("Enter char to continue\n");
        // c = getchar();
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
    if(intel->ram_counter >= RAM_MAX_SIZE){
        fprintf(stderr, "RAM is full - can't add elem to ram");
        intel->ic += 2;
        return RAM_OVERFLOW;
    }
    int temp = 0;
    intel->ic++;

    intel->RAM[intel->ram_counter] = intel->registr[(intel->code.comands)[intel->ic]];
    intel->ic++;
    intel->ram_counter++;

    return NO_MISTAKE;
}

static stack_err_bytes pushm(processor* intel){
    int temp = 0;
    intel->ram_counter--;

    intel->ic++;
    temp = intel->RAM[intel->ram_counter];

    CHECK_STACK_ERR(stack_push(intel->stack, &temp));
    intel->ic++;

    intel->ram_counter++;
    return NO_MISTAKE;
}

static stack_err_bytes in(processor* intel){
    int temp = 0;
    printf("Input coefficent of square equation:\n");
    scanf("%d", &temp);
    CHECK_STACK_ERR(stack_push(intel->stack, &temp));
    intel->ic++;
    return NO_MISTAKE;
}

static stack_err_bytes out(processor* intel, int* result){
    CHECK_STACK_ERR(stack_pop(intel->stack, result));
    printf("result = %d\n", *result);
    intel->ic++;
    return NO_MISTAKE;
}

static stack_err_bytes add(processor* intel){
    int pop = 0;
    int temp = 0;
    for(int idx = 0; idx < 2; idx ++){
        CHECK_STACK_ERR(stack_pop(intel->stack, &pop));
        temp += pop;
    }
    CHECK_STACK_ERR(stack_push(intel->stack, &temp));
    intel->ic++;
    return NO_MISTAKE;
}

static stack_err_bytes mul(processor* intel){
    int pop = 0;
    int temp = 1;
    for(int idx = 0; idx < 2; idx ++){
        CHECK_STACK_ERR(stack_pop(intel->stack, &pop));
        temp *= pop;
    }
    CHECK_STACK_ERR(stack_push(intel->stack, &temp));
    intel->ic++;
    return NO_MISTAKE;
}

static stack_err_bytes sub(processor* intel){
    int pop = 0;
    int temp = 0;

    CHECK_STACK_ERR(stack_pop(intel->stack, &pop));
    temp -= pop;

    CHECK_STACK_ERR(stack_pop(intel->stack, &pop));
    temp += pop;

    CHECK_STACK_ERR(stack_push(intel->stack, &temp));
    intel->ic++;
    return NO_MISTAKE;
}

static stack_err_bytes div(processor* intel){
    int pop = 0;
    double temp = 1;

    CHECK_STACK_ERR(stack_pop(intel->stack, &pop));
    temp /= (double)pop;

    CHECK_STACK_ERR(stack_pop(intel->stack, &pop));
    temp *= pop;

    pop = temp;
    CHECK_STACK_ERR(stack_push(intel->stack, &pop));

    intel->ic++;
    return NO_MISTAKE;
}

static stack_err_bytes sqrt(processor* intel){
    int pop = 0;
    double temp = 0;

    CHECK_STACK_ERR(stack_pop(intel->stack, &pop));

    if(pop >= 0){
        temp = sqrt(pop);
        pop = round(temp);
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
    intel -> ic = intel->code.comands[intel -> ic]; 

    return NO_MISTAKE;
}

static stack_err_bytes ret(processor* intel){
    int idx = 0;
    CHECK_STACK_ERR(stack_pop(intel->call_stack, &idx));
    intel->ic = idx; //да будет ругаться, что из int в size_t но стек на интах и тут ничего не поделаешь

    return NO_MISTAKE;
}


static stack_err_bytes jump_if_condition_sw(processor* intel, bool condition){
    if(condition){
        intel -> ic = intel->code.comands[intel -> ic + 1];
        return NO_MISTAKE;
    }
    intel->ic++; // перепрыгиваем на следуюбщий элемент - номер строки
    intel->ic++; // перепрыгиваем на следующую команду
    return NO_MISTAKE;
}
