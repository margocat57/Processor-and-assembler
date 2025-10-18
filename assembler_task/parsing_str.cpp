#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "parsing_str.h"

static assembler_err_t parse_cmnds(assembler* assembl);

static assembler_err_t pushrm_poprm(int cmd, assembler* assembl);

static void func_with_metka(int cmd, assembler* assembl);

static void push(int cmd, assembler* assembl);

static char* skip_space(char* current_str){
    char* str_without_space = current_str;
    str_without_space = str_without_space + strspn(current_str, " \t\n\r\f\v");
    return str_without_space;
}

listing* fill_listing_struct(assembler* assembl){
    listing* info = (listing*)calloc(assembl->file_in_arr.amount_str * 2, sizeof(listing));
    if(!info){
        return NULL;
    }

    // сделать соовевтие строк и адресов - и передавать как элемент ассемблера
    for (size_t idx = 0; idx < assembl->file_in_arr.amount_str; idx++){
        for(size_t cmd = 1; cmd < AMNT_CMD; cmd++){
            assembl->ptr_array[idx] = skip_space(assembl->ptr_array[idx]);
            // В отдельную функцию, она будет простой
            if(!COMANDS[cmd].name_of_comand){
                continue;
            }
            if(COMANDS[cmd].num_of_params >= 1 &&
                !strncmp(assembl->ptr_array[idx], COMANDS[cmd].name_of_comand, COMANDS[cmd].size)){
                (info + idx)->instruction = assembl->ptr_array[idx];
                (info + idx)->pc = assembl->asm_bytecode_size;
                (assembl->asm_bytecode_size)+=2;
                break;
            }
            else if(!strncmp(assembl->ptr_array[idx], COMANDS[cmd].name_of_comand, COMANDS[cmd].size)){
                (info + idx)->instruction = assembl->ptr_array[idx];
                (info + idx)->pc = assembl->asm_bytecode_size;
                (assembl->asm_bytecode_size)++;
                break;
            }
            else if(strchr(assembl->ptr_array[idx], ':')){
                (info + idx)->instruction = assembl->ptr_array[idx];
                (info + idx)->pc = -1;
                break;
            }
        }
    }
    return info;
}

//  Зачем возвращать указатель на структуру, ты её и так меняешь по указателю
// Лучше возвращать ошибку!
assembler_err_t parser(assembler* assembl){
    assembler_err_t err = NO_MISTAKE;
    if(!assembl->metki_asm.metki_arr){
        fprintf(stderr, "Can't use metki ");
        return ALLOC_ERROR;
    }
    // После рефакторинга metki уже будешь знать кол-во команд
    int* arr_with_code = (int*)calloc(assembl->file_in_arr.amount_str * 2, sizeof(int));
    if(!arr_with_code){
        fprintf(stderr, "Can't allocate memory for bytecode array");
        return ALLOC_ERROR;
    }
    assembl->bytecode = arr_with_code;
    fprintf(stderr, "%d\n", assembl->file_in_arr.amount_str);

    for( ; assembl->asm_pc < assembl->file_in_arr.amount_str; assembl->asm_pc++){
        if(!assembl->info[assembl->asm_pc].instruction){
            fprintf(stderr, "current ptr is null, parsing stopped");
            break;
        }

        err = parse_cmnds(assembl);
        if(err){
            return err;
        }
    }

    // TODO DEBUG_FOR (можно asm_dump())
    for(int i = 0; i < assembl->asm_bytecode_size; i++){
        fprintf(stderr, "[%d]: %d\n", i, arr_with_code[i]);
    }

    return NO_MISTAKE;
}

static assembler_err_t parse_cmnds(assembler* assembl){
    size_t length = 0;
    for(size_t cmd = 1; cmd < AMNT_CMD; cmd++){
        length = strcspn(assembl->info[assembl->asm_pc].instruction, " \t\n\r\f\v");
        // вот здесь свич
        if(!COMANDS[cmd].name_of_comand || length != COMANDS[cmd].size || strncmp(assembl->info[assembl->asm_pc].instruction, COMANDS[cmd].name_of_comand, COMANDS[cmd].size) 
        || assembl->info[assembl->asm_pc].pc != -1 ){
            continue;
        }

        switch(COMANDS[cmd].elem_type){
            case PUSHRM_POPRM:
                return pushrm_poprm(cmd, assembl);

            case JUMP_WITH_COND:
                func_with_metka(cmd, assembl);
                break;

            case PUSH_TYPE:
                push(cmd, assembl);
                break;

            case OTHER:
                assembl->bytecode[assembl->info[assembl->asm_pc].pc] = COMANDS[cmd].bytecode;
                break;

            default:
                return INCORRECT_CMD;
        }
    }

    return NO_MISTAKE;
}

static void push(int cmd, assembler* assembl){
    //TODO адекватная адресация
    assembl->bytecode[assembl->info[assembl->asm_pc].pc] = cmd;
    char* current_str = assembl->info[assembl->asm_pc].instruction + COMANDS[cmd].size + 1;

    assembl->asm_pc++;
    assembl->bytecode[assembl->info[assembl->asm_pc].pc + 1] = atoi(current_str);
}

static void func_with_metka(int cmd, assembler* assembl){
    assembl->bytecode[assembl->info[assembl->asm_pc].pc] = cmd;

    char* current_str = assembl->info[assembl->asm_pc].instruction;
    current_str = strchr(current_str, ':'); // доходим до метки
    current_str++; // доходим до числа

    assembl->bytecode[assembl->info[assembl->asm_pc].pc + 1] = assembl->metki_asm.metki_arr[atoi(current_str)];
}

static assembler_err_t pushrm_poprm(int cmd, assembler* assembl){
    assembl->bytecode[assembl->info[assembl->asm_pc].pc] = cmd;

    char* current_str = assembl->info[assembl->asm_pc].instruction + COMANDS[cmd].size + 1;

    current_str = strchr(current_str, 'X') - 1;
    if ('A' > current_str[0] || current_str[0] > 'P'){
        fprintf(stderr, "Incorrect registr R%cXin", current_str[0]);
        return INCORRECT_REGISTR;
    }

    assembl->bytecode[assembl->info[assembl->asm_pc].pc + 1] = current_str[0] - 'A';
    return NO_MISTAKE;
}