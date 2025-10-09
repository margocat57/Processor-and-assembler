#include <stdio.h>
#include <math.h>
#include "parse_asm_from_file.h"
#include "assembler_task/parsing_str.h"
#include "calcul.h"
#include "stack_for_calcul/stack_func.h"
#include "stack_for_calcul/my_assert.h"
#include "stack_for_calcul/stack.h"
#include "parse_asm_from_file.h"

#define JUMP_IF(intel, comp) \
    do { \
        int t1 = 0, t2 = 0; \
        stack_pop((intel)->stack, &t1); \
        stack_pop((intel)->stack, &t2); \
        stack_push((intel)->stack, &t1); \
        stack_push((intel)->stack, &t2); \
        jump_if_condition_sw(intel, t1 comp t2); \
    } while(0)

#define LESS    <
#define LESS_EQ <=
#define MORE    >
#define MORE_EQ >=
#define EQ      ==
#define NOT_EQ  !=

#define DO_CASE(function) \
    function; \
    break; \

static void proc_push(processor* intel);

static void add(processor* intel);

static void mul(processor* intel);

static void div(processor* intel);

static void sub(processor* intel);

static void sqrt(processor* intel);

static void jump_if_condition(processor* intel, bool (*compare_suc)(int, int));

static void jump_if_condition_sw(processor*intel, bool condition);

static void out(processor* intel, int* result);

static void in(processor* intel);

static void popr(processor* intel);

static void pushr(processor* intel);

// --------------------------------------
// TODO файлик с реализациями компараторов 
bool eq(int a, int b){
    return a == b ? true : false;
}

bool neq(int a, int b){
    return a != b ? true : false;
}

bool more(int a, int b){
    return a > b ? true : false;
}

bool more_eq(int a, int b){
    return a >= b ? true : false;
}

bool less(int a, int b){
    return a < b ? true : false;
}

bool less_eq(int a, int b){
    return a <= b ? true : false;
}
// --------------------------------------

stack_err_bytes calculate(processor* intel){
    stack_err_bytes res = NO_MISTAKE;
    res = processor_verify(intel);
    if(res){
        return res;
    }

    int result = 0;
    char c = 'o'; // for pause debug
    // processor_dump(intel);

    // TODO: tabli4ka
    for(; intel->ic < intel->code.size;){
        // printf("before:\n");
        // stack_dump(intel->stack);
        // printf("%d\n", intel->code.comands[intel->ic]);
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
        case VLT:   return processor_verify(intel);
        default:
            fprintf(stderr, "INCORRECT CMD CODE");
            return res;
        }
        // begin DEBUG code
        // printf("after:\n");
        // stack_dump(intel->stack);
        // printf("Enter char to continue\n");
        // c = getchar();
        // end DEBUG code
    }
    res = processor_verify(intel);
    return res;
}

static void proc_push(processor* intel){
    stack_push(intel->stack, &(intel->code.comands[intel->ic + 1]));
    intel->ic++;
    intel->ic++;
}


static void popr(processor* intel){
    int temp = 0;
    intel->ic++;
    stack_pop(intel->stack, &temp);
    intel->registr[(intel->code.comands)[intel->ic]] = temp;
    intel->ic++;
}

static void pushr(processor* intel){
    int temp = 0;
    intel->ic++;
    temp = intel->registr[(intel->code.comands)[intel->ic]];
    stack_push(intel->stack, &temp);
    intel->ic++;
}

static void in(processor* intel){
    int temp = 0;
    printf("Input coefficent of square equation:\n");
    scanf("%d", &temp);
    stack_push(intel->stack, &temp);
    intel->ic++;
}

static void out(processor* intel, int* result){
    stack_pop(intel->stack, result);
    printf("result = %d\n", *result);
    intel->ic++;
}

static void add(processor* intel){
    int pop = 0;
    int temp = 0;
    for(int idx = 0; idx < 2; idx ++){
        stack_pop(intel->stack, &pop);
        temp += pop;
    }
    stack_push(intel->stack, &temp);
    intel->ic++;
}

static void mul(processor* intel){
    int pop = 0;
    int temp = 1;
    for(int idx = 0; idx < 2; idx ++){
        stack_pop(intel->stack, &pop);
        temp *= pop;
    }
    stack_push(intel->stack, &temp);
    intel->ic++;
}

static void sub(processor* intel){
    int pop = 0;
    int temp = 0;

    stack_pop(intel->stack, &pop);
    temp -= pop;

    stack_pop(intel->stack, &pop);
    temp += pop;

    stack_push(intel->stack, &temp);
    intel->ic++;
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

    intel->ic++;
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

    intel->ic++;
}

// компаратор!!!
// можно вставить свитч и сделать енам с условиями и тут передавать условие как некое число из енама
// допустим если нам передали JB то  мы вызываем переходим к соотвествующему условию свича и вызываем конкретную функцию

// медленно дед вероятно не зачтет
static void jump_if_condition(processor* intel, bool (*compare_suc)(int, int)){
    int temp1 = 0;
    int temp2 = 0;
    stack_pop(intel->stack, &temp1);
    stack_pop(intel->stack, &temp2);
    if(compare_suc(temp1, temp2)){
        intel -> ic = intel->code.comands[intel -> ic + 1];
        return;
    }
    intel->ic++; // перепрыгиваем на следуюбщий элемент - номер строки
    intel->ic++; // перепрыгиваем на следующую команду
}

// функция с дефайнами как ассерт на стеке
// версия 6 функций - обязательно

static void jump_if_condition_sw(processor* intel, bool condition){
    if(condition){
        intel -> ic = intel->code.comands[intel -> ic + 1];
        return;
    }
    intel->ic++; // перепрыгиваем на следуюбщий элемент - номер строки
    intel->ic++; // перепрыгиваем на следующую команду
}

/*
6 функций чтобы были
static void more(processor* intel{
    int temp1 = 0;
    int temp2 = 0;
    stack_pop(intel->stack, &temp1);
    stack_pop(intel->stack, &temp2);
    if(temp1 > temp2)){
        intel -> ic = intel->code.comands[intel -> ic + 1];
        return;
    }
    intel->ic++; // перепрыгиваем на следуюбщий элемент - номер строки
    intel->ic++; // перепрыгиваем на следующую команду
}

static void more_eq(processor* intel{
    int temp1 = 0;
    int temp2 = 0;
    stack_pop(intel->stack, &temp1);
    stack_pop(intel->stack, &temp2);
    if(temp1 >= temp2)){
        intel -> ic = intel->code.comands[intel -> ic + 1];
        return;
    }
    intel->ic++; // перепрыгиваем на следуюбщий элемент - номер строки
    intel->ic++; // перепрыгиваем на следующую команду
}

static void less(processor* intel{
    int temp1 = 0;
    int temp2 = 0;
    stack_pop(intel->stack, &temp1);
    stack_pop(intel->stack, &temp2);
    if(temp1 < temp2)){
        intel -> ic = intel->code.comands[intel -> ic + 1];
        return;
    }
    intel->ic++; // перепрыгиваем на следуюбщий элемент - номер строки
    intel->ic++; // перепрыгиваем на следующую команду
}

static void less_eq(processor* intel{
    int temp1 = 0;
    int temp2 = 0;
    stack_pop(intel->stack, &temp1);
    stack_pop(intel->stack, &temp2);
    if(temp1 <= temp2)){
        intel -> ic = intel->code.comands[intel -> ic + 1];
        return;
    }
    intel->ic++; // перепрыгиваем на следуюбщий элемент - номер строки
    intel->ic++; // перепрыгиваем на следующую команду
}

static void eq(processor* intel{
    int temp1 = 0;
    int temp2 = 0;
    stack_pop(intel->stack, &temp1);
    stack_pop(intel->stack, &temp2);
    if(temp1 == temp2)){
        intel -> ic = intel->code.comands[intel -> ic + 1];
        return;
    }
    intel->ic++; // перепрыгиваем на следуюбщий элемент - номер строки
    intel->ic++; // перепрыгиваем на следующую команду
}

static void not_eq(processor* intel{
    int temp1 = 0;
    int temp2 = 0;
    stack_pop(intel->stack, &temp1);
    stack_pop(intel->stack, &temp2);
    if(temp1 != temp2)){
        intel -> ic = intel->code.comands[intel -> ic + 1];
        return;
    }
    intel->ic++; // перепрыгиваем на следуюбщий элемент - номер строки
    intel->ic++; // перепрыгиваем на следующую команду
}
*/