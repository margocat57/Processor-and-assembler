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
#include "../cmd_info.h"
#include "../stack_for_calcul/log.h"
#include "color_lib_proc.h"

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
create_func_arithm_operations(/, divide);

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
        if (bytecode_elem >= AMNT_CMD || !COMANDS[bytecode_elem].function_proc){
            fprintf(stderr, "INCORRECT COMAND");
            return INCORR_COMAND;
        }
        res = COMANDS[bytecode_elem].function_proc(intel);
        if(res){
            return res;
        }
        // stack_dump(intel->stack);
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

stack_err_bytes draw(processor* intel){
    for(size_t idx_ram = 0; idx_ram < RAM_MAX_SIZE; idx_ram++){
        if (intel->RAM[idx_ram] == 1) {
        printf_to_log_file(BLUE_LIGHT "* " COLOR_RESET);
        } 
        else {
        printf_to_log_file("  ");
        }

        if((idx_ram + 1) % 10 == 0){
            printf_to_log_file("\n");
        }
    }
    intel->ic++;
    return NO_MISTAKE;
}

stack_err_bytes proc_push(processor* intel){
    CHECK_STACK_ERR(stack_push(intel->stack, &(intel->code.comands[intel->ic + 1])));
    intel->ic++;
    intel->ic++;
    return NO_MISTAKE;
}


stack_err_bytes popr(processor* intel){
    int temp = 0;
    intel->ic++;
    CHECK_STACK_ERR(stack_pop(intel->stack, &temp));  
    intel->registr[(intel->code.comands)[intel->ic]] = temp;
    intel->ic++;
    return NO_MISTAKE;
}

stack_err_bytes pushr(processor* intel){
    int temp = 0;
    intel->ic++;
    temp = intel->registr[(intel->code.comands)[intel->ic]];
    CHECK_STACK_ERR(stack_push(intel->stack, &temp));
    intel->ic++;
    return NO_MISTAKE;
}

stack_err_bytes popm(processor* intel){
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

stack_err_bytes pushm(processor* intel){
    int temp = 0;

    intel->ic++;
    intel->ram_counter = intel->registr[(intel->code.comands)[intel->ic]];
    temp = intel->RAM[intel->ram_counter];

    CHECK_STACK_ERR(stack_push(intel->stack, &temp));
    intel->ic++;

    return NO_MISTAKE;
}

stack_err_bytes in(processor* intel){
    int temp = 0;
    printf("Input parametr:\n");
    scanf("%d", &temp);
    CHECK_STACK_ERR(stack_push(intel->stack, &temp));
    intel->ic++;
    return NO_MISTAKE;
}

stack_err_bytes out(processor* intel){
    CHECK_STACK_ERR(stack_pop(intel->stack, &intel->result));
    printf("result = %d\n", intel->result);
    intel->ic++;
    return NO_MISTAKE;
}

stack_err_bytes sqrt(processor* intel){
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

stack_err_bytes call(processor* intel){
    intel->ic += 2; // увеличиваем счетчик на 2 чтобы добраться до команды, которая следуер за call и меткой
    int return_address = (int)intel->ic;
    CHECK_STACK_ERR(stack_push(intel->call_stack, &return_address));

    intel->ic--; //  перескакиваем на прошлую команду
    intel -> ic = (size_t)intel->code.comands[intel -> ic]; 

    return NO_MISTAKE;
}

stack_err_bytes ret(processor* intel){
    int idx = 0;
    CHECK_STACK_ERR(stack_pop(intel->call_stack, &idx));
    intel->ic = (size_t)idx; //да будет ругаться, что из int в size_t но стек на интах и тут ничего не поделаешь

    return NO_MISTAKE;
}


stack_err_bytes jmp(processor* intel){
    intel -> ic = (size_t)intel->code.comands[intel -> ic + 1];
    return NO_MISTAKE;
}

/* макрос - генерация функций
#define create_func(operation, name) \
    int name(int a, int b) { \
        return a operation b; \
    }

create_func( + , add)
*/