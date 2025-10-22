//!@file
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "parsing_str.h"
#include "../stack_for_calcul/hash.h"
#include "../cmd_info.h"


//хэш-необходимое но недостаточное условие - юзать strcmp

static void put_params(instruction_info* info, assembler* assembl, long long int pc_in_bytecode_arr, int idx, size_t hash);

//! @brief Dispatches command parsing to appropriate handler based on command type
//!
//! Operation:
//! 1. Validates assembler pointer
//! 2. Iterates through all known commands (COMANDS array)
//! 3. Compares instruction length and content with command definitions
//! 4. Skips labels (pc == -1) and non-matching commands
//! 5. Routes matching commands to appropriate parser based on elem_type:
//!    - PUSHRM_POPRM: pushrm_poprm() for register/arm operations
//!    - JUMP_WITH_COND: func_with_metka() for conditional jumps
//!    - PUSH_TYPE: push() for push operations
//!    - OTHER: other() for all other commands
//! 6. Returns INCORRECT_CMD for unknown command types
static assembler_err_t parse_cmnds(assembler* assembl);

static char* skip_space(char* current_str){
    if(!current_str){
        fprintf(stderr, "Can't work - NULL current_str ptr");
        return NULL;
    }

    char* str_without_space = current_str;
    str_without_space = str_without_space + strspn(current_str, " \t\n\r\f\v");
    return str_without_space;
}

instruction_info* fill_listing_struct(assembler* assembl){
    if(!assembl){
        fprintf(stderr, "Can't work - NULL assembler ptr");
        return NULL;
    }

    instruction_info* info = (instruction_info*)calloc(assembl->file_in_arr.amount_str, sizeof(instruction_info));
    if(!info){
        fprintf(stderr, "Can't alloc memory for info arr");
        return NULL;
    }
    long long int no_cmd = -1;
    size_t length = 0;
    size_t cmd_hash = 0;

    for (int idx = 0; idx < (int)assembl->file_in_arr.amount_str; idx++){
        if(strchr(assembl->ptr_array[idx], ':')){
            put_params(info, assembl, no_cmd, idx, cmd_hash);
            continue;
        }

        assembl->ptr_array[idx] = skip_space(assembl->ptr_array[idx]);
        length = strcspn(assembl->ptr_array[idx], " \t\n\r\f\v");
        cmd_hash = create_djb2_hash(assembl->ptr_array[idx], length);

        for(int cmd = 1; cmd < (int)AMNT_CMD; cmd++){
            if(!COMANDS[cmd].name_of_comand || !assembl->ptr_array[idx]){
                continue;
            }

            if(cmd_hash == COMANDS[cmd].hash){
                if(strncmp(COMANDS[cmd].name_of_comand, assembl->ptr_array[idx], COMANDS[cmd].size)){
                    continue;
                }
                put_params(info, assembl, (long long int)assembl->asm_bytecode_size, idx, cmd_hash);
                (assembl->asm_bytecode_size)++;
                if(COMANDS[cmd].num_of_params >= 1){
                    (assembl->asm_bytecode_size)++;
                }
                break;
            }
        }
    }
    return info;
}

static void put_params(instruction_info* info, assembler* assembl, long long int pc_in_bytecode_arr, int idx, size_t hash){
    (info + idx)->instruction = assembl->ptr_array[idx];
    (info + idx)->pc = pc_in_bytecode_arr;
    (info + idx)->hash = hash;
}

assembler_err_t parser(assembler* assembl){
    assembler_err_t err = NO_MISTAKE_ASM;
    if(!assembl){
        fprintf(stderr, "Can't work - NULL assembler ptr");
        return NULL_PTR;
    }

    if(!assembl->metki_asm.metki_arr){
        fprintf(stderr, "Can't use metki ");
        return ALLOC_ERROR_ASM;
    }

    int* arr_with_code = (int*)calloc(assembl->asm_bytecode_size, sizeof(int));
    if(!arr_with_code){
        fprintf(stderr, "Can't allocate memory for bytecode array");
        return ALLOC_ERROR_ASM;
    }
    assembl->bytecode = arr_with_code;

    for( ; assembl->asm_pc < assembl->file_in_arr.amount_str; assembl->asm_pc++){
        if(!assembl->info[assembl->asm_pc].instruction){
            continue;
        }

        err = parse_cmnds(assembl);
        if(err){
            return err;
        }
    }

    return NO_MISTAKE_ASM;
}

static assembler_err_t parse_cmnds(assembler* assembl){
    assembler_err_t err = NO_MISTAKE_ASM;
    if(!assembl){
        fprintf(stderr, "Can't work - NULL assembler ptr");
        return NULL_PTR;
    }

    size_t length = 0;
    for(int cmd = 1; cmd < (int)AMNT_CMD; cmd++){
        // а стоит здесь считать длину
        length = strcspn(assembl->info[assembl->asm_pc].instruction, " \t\n\r\f\v");

        if(!COMANDS[cmd].name_of_comand || length != COMANDS[cmd].size || assembl->info[assembl->asm_pc].pc == -1 ||
        assembl->info[assembl->asm_pc].hash != COMANDS[cmd].hash){
            continue;
        }
        if(strncmp(COMANDS[cmd].name_of_comand, assembl->info[assembl->asm_pc].instruction, COMANDS[cmd].size)){
            continue;
        }
        err = COMANDS[cmd].function_asm(cmd, assembl);
        if(err){
            return err;
        }
    }

    return NO_MISTAKE_ASM;
}

assembler_err_t other(int cmd, assembler* assembl){
    size_t number_of_str_in_txt_file = assembl->asm_pc;
    long long int index_of_bytecode_array = assembl->info[number_of_str_in_txt_file].pc;

    assembl->bytecode[index_of_bytecode_array] = cmd;
    assembl->info[assembl->asm_pc].bytecode = assembl->bytecode[index_of_bytecode_array];

    assembl->info[assembl->asm_pc].num_of_args = COMANDS[cmd].num_of_params;
    return NO_MISTAKE_ASM;
}

assembler_err_t push(int cmd, assembler* assembl){
    size_t number_of_str_in_txt_file = assembl->asm_pc;
    long long int index_of_bytecode_array = assembl->info[number_of_str_in_txt_file].pc;

    assembl->bytecode[index_of_bytecode_array] = cmd;
    assembl->info[assembl->asm_pc].bytecode = assembl->bytecode[index_of_bytecode_array];

    assembl->info[assembl->asm_pc].num_of_args = COMANDS[cmd].num_of_params;

    char* current_str = assembl->info[assembl->asm_pc].instruction + COMANDS[cmd].size + 1;
    index_of_bytecode_array = assembl->info[number_of_str_in_txt_file].pc + 1;

    assembl->bytecode[index_of_bytecode_array] = atoi(current_str);
    assembl->info[assembl->asm_pc].args = assembl->bytecode[index_of_bytecode_array];

    return NO_MISTAKE_ASM;
}

assembler_err_t func_with_metka(int cmd, assembler* assembl){
    size_t number_of_str_in_txt_file = assembl->asm_pc;
    long long int index_of_bytecode_array = assembl->info[number_of_str_in_txt_file].pc;

    assembl->bytecode[index_of_bytecode_array] = cmd;
    assembl->info[assembl->asm_pc].bytecode = assembl->bytecode[index_of_bytecode_array];

    assembl->info[assembl->asm_pc].num_of_args = COMANDS[cmd].num_of_params;

    char* current_str = assembl->info[assembl->asm_pc].instruction;
    current_str = strchr(current_str, ':'); // доходим до метки
    current_str++; // доходим до числа

    index_of_bytecode_array = assembl->info[number_of_str_in_txt_file].pc + 1;
    assembl->bytecode[index_of_bytecode_array] = assembl->metki_asm.metki_arr[atoi(current_str)];
    assembl->info[assembl->asm_pc].args = assembl->bytecode[index_of_bytecode_array];

    return NO_MISTAKE_ASM;
}

// добавить 2 функции с проверкой допустимости регистров
assembler_err_t pushr_popr(int cmd, assembler* assembl){
    size_t number_of_str_in_txt_file = assembl->asm_pc;
    long long int index_of_bytecode_array = assembl->info[number_of_str_in_txt_file].pc;

    assembl->bytecode[index_of_bytecode_array] = cmd;
    assembl->info[assembl->asm_pc].bytecode = assembl->bytecode[index_of_bytecode_array];

    assembl->info[assembl->asm_pc].num_of_args = COMANDS[cmd].num_of_params;

    char* current_str = assembl->info[assembl->asm_pc].instruction + COMANDS[cmd].size + 1;

    current_str = strchr(current_str, 'R');
    if(!current_str){
        fprintf(stderr, "Incorrect registr, can't fing R in word");
        return INCORRECT_REGISTR;
    }
    current_str = strchr(current_str, 'X');
    if(!current_str){
        fprintf(stderr, "Incorrect registr, can't fing X in word");
        return INCORRECT_REGISTR;
    }

    current_str--;
    if ('A' > current_str[0] || current_str[0] > 'P'){
        fprintf(stderr, "Incorrect registr R%cX", current_str[0]);
        return INCORRECT_REGISTR;
    }

    index_of_bytecode_array = assembl->info[number_of_str_in_txt_file].pc + 1;
    assembl->bytecode[index_of_bytecode_array] = current_str[0] - 'A';

    assembl->info[assembl->asm_pc].args = assembl->bytecode[index_of_bytecode_array];
    return NO_MISTAKE_ASM;
}

assembler_err_t pushm_popm(int cmd, assembler* assembl){
    size_t number_of_str_in_txt_file = assembl->asm_pc;
    long long int index_of_bytecode_array = assembl->info[number_of_str_in_txt_file].pc;

    assembl->bytecode[index_of_bytecode_array] = cmd;
    assembl->info[assembl->asm_pc].bytecode = assembl->bytecode[index_of_bytecode_array];

    assembl->info[assembl->asm_pc].num_of_args = COMANDS[cmd].num_of_params;

    char* current_str = assembl->info[assembl->asm_pc].instruction + COMANDS[cmd].size + 1;

    current_str = strchr(current_str, 'X');
    if(!current_str){
        fprintf(stderr, "Incorrect registr");
        return INCORRECT_REGISTR;
    }
    current_str--;
    if ('A' > current_str[0] || current_str[0] > 'P'){
        fprintf(stderr, "Incorrect registr R%cX", current_str[0]);
        return INCORRECT_REGISTR;
    }

    index_of_bytecode_array = assembl->info[number_of_str_in_txt_file].pc + 1;
    assembl->bytecode[index_of_bytecode_array] = current_str[0] - 'A';

    assembl->info[assembl->asm_pc].args = assembl->bytecode[index_of_bytecode_array];
    return NO_MISTAKE_ASM;
}