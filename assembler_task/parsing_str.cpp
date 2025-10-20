//!@file
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "parsing_str.h"

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

//! @brief Processes register-based PUSH/POP operations
//!
//! For register/ram operations:
//! - Writes command bytecode to array and listing
//! - Extracts register name from instruction 
//! - Validates register letter is in range of max number of registers
//! - Returns error if register is outside valid range
//! - Converts register letter to numeric index
//! - Writes register index to NEXT position in bytecode (pc + 1)
static assembler_err_t pushrm_poprm(int cmd, assembler* assembl);

//! @brief Processes jump commands with label targets
//!
//! For conditional/unconditional jumps:
//! - Writes jump command bytecode to bytecode array  
//! - Stores command in listing structure
//! - Extracts label number from instruction (text after colon)
//! - Looks up the target address from  metki array
//! - Writes the resolved address to NEXT position in bytecode (pc + 1)
//! - Stores both command and target address in listing
static void func_with_metka(int cmd, assembler* assembl);

//! @brief Processes PUSH commands with immediate numeric values
//!
//! For PUSH commands with direct values:
//! - Writes the PUSH command bytecode to bytecode array
//! - Stores the command in listing structure
//! - Extracts numeric value from instruction string (after command name + space)
//! - Writes the numeric argument to the next position in bytecode array (pc + 1)
//! - Stores both command and argument in listing array
static void push(int cmd, assembler* assembl);

//! @brief Processes simple commands without arguments (e.g., ADD, SUB, MUL)
//! 
//! For commands that don't take any arguments:
//! - Writes the command bytecode to the main bytecode array
//! - Stores the same bytecode in the listing structure for debugging
//! - Sets num_of_args to 0 in the listing array
static void other(int cmd, assembler* assembl);

static void put_params(listing* info, assembler* assembl, long long int pc_in_bytecode_arr, int idx);

static char* skip_space(char* current_str){
    if(!current_str){
        fprintf(stderr, "Can't work - NULL current_str ptr");
        return NULL;
    }

    char* str_without_space = current_str;
    str_without_space = str_without_space + strspn(current_str, " \t\n\r\f\v");
    return str_without_space;
}

listing* fill_listing_struct(assembler* assembl){
    if(!assembl){
        fprintf(stderr, "Can't work - NULL assembler ptr");
        return NULL;
    }

    listing* info = (listing*)calloc(assembl->file_in_arr.amount_str, sizeof(listing));
    if(!info){
        fprintf(stderr, "Can't alloc memory for info arr");
        return NULL;
    }
    long long int no_cmd = -1;

    for (int idx = 0; idx < (int)assembl->file_in_arr.amount_str; idx++){
        for(int cmd = 1; cmd < (int)AMNT_CMD; cmd++){
            if(!COMANDS[cmd].name_of_comand || !assembl->ptr_array[idx]){
                continue;
            }
            assembl->ptr_array[idx] = skip_space(assembl->ptr_array[idx]);
            if(COMANDS[cmd].num_of_params >= 1 &&
                !strncmp(assembl->ptr_array[idx], COMANDS[cmd].name_of_comand, COMANDS[cmd].size)){
                put_params(info, assembl, (long long int)assembl->asm_bytecode_size, idx);
                (assembl->asm_bytecode_size) += 2;
                break;
            }
            else if(!strncmp(assembl->ptr_array[idx], COMANDS[cmd].name_of_comand, COMANDS[cmd].size)){
                put_params(info, assembl, (long long int)assembl->asm_bytecode_size, idx);
                (assembl->asm_bytecode_size)++;
                break;
            }
            else if(!strncmp(assembl->ptr_array[idx], ":", 1)){
                put_params(info, assembl, no_cmd, idx);
                break;
            }
        }
    }
    return info;
}

static void put_params(listing* info, assembler* assembl, long long int pc_in_bytecode_arr, int idx){
    (info + idx)->instruction = assembl->ptr_array[idx];
    (info + idx)->pc = pc_in_bytecode_arr;
}

assembler_err_t parser(assembler* assembl){
    assembler_err_t err =NO_MISTAKE_ASM;
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
    if(!assembl){
        fprintf(stderr, "Can't work - NULL assembler ptr");
        return NULL_PTR;
    }

    size_t length = 0;
    for(int cmd = 1; cmd < (int)AMNT_CMD; cmd++){
        length = strcspn(assembl->info[assembl->asm_pc].instruction, " \t\n\r\f\v");

        if(!COMANDS[cmd].name_of_comand || length != COMANDS[cmd].size || assembl->info[assembl->asm_pc].pc == -1 
        || strncmp(assembl->info[assembl->asm_pc].instruction, COMANDS[cmd].name_of_comand, COMANDS[cmd].size) ){
            continue;
        }

        switch(COMANDS[cmd].elem_type){
            case PUSHRM_POPRM:   return pushrm_poprm(cmd, assembl);
            case JUMP_WITH_COND: func_with_metka(cmd, assembl); break;
            case PUSH_TYPE:      push(cmd, assembl);            break;
            case OTHER:          other(cmd, assembl);           break;
            default:             return INCORRECT_CMD;
        }
    }

    return NO_MISTAKE_ASM;
}

static void other(int cmd, assembler* assembl){
    size_t number_of_str_in_txt_file = assembl->asm_pc;
    long long int index_of_bytecode_array = assembl->info[number_of_str_in_txt_file].pc;

    assembl->bytecode[index_of_bytecode_array] = cmd;
    assembl->info[assembl->asm_pc].bytecode = assembl->bytecode[index_of_bytecode_array];

    assembl->info[assembl->asm_pc].num_of_args = COMANDS[cmd].num_of_params;
}

static void push(int cmd, assembler* assembl){
    size_t number_of_str_in_txt_file = assembl->asm_pc;
    long long int index_of_bytecode_array = assembl->info[number_of_str_in_txt_file].pc;

    assembl->bytecode[index_of_bytecode_array] = cmd;
    assembl->info[assembl->asm_pc].bytecode = assembl->bytecode[index_of_bytecode_array];

    assembl->info[assembl->asm_pc].num_of_args = COMANDS[cmd].num_of_params;

    char* current_str = assembl->info[assembl->asm_pc].instruction + COMANDS[cmd].size + 1;
    index_of_bytecode_array = assembl->info[number_of_str_in_txt_file].pc + 1;

    assembl->bytecode[index_of_bytecode_array] = atoi(current_str);
    assembl->info[assembl->asm_pc].args = assembl->bytecode[index_of_bytecode_array];
}

static void func_with_metka(int cmd, assembler* assembl){
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
}

static assembler_err_t pushrm_poprm(int cmd, assembler* assembl){
    size_t number_of_str_in_txt_file = assembl->asm_pc;
    long long int index_of_bytecode_array = assembl->info[number_of_str_in_txt_file].pc;

    assembl->bytecode[index_of_bytecode_array] = cmd;
    assembl->info[assembl->asm_pc].bytecode = assembl->bytecode[index_of_bytecode_array];

    assembl->info[assembl->asm_pc].num_of_args = COMANDS[cmd].num_of_params;

    char* current_str = assembl->info[assembl->asm_pc].instruction + COMANDS[cmd].size + 1;

    current_str = strchr(current_str, 'X') - 1;
    if ('A' > current_str[0] || current_str[0] > 'P'){
        fprintf(stderr, "Incorrect registr R%cXin", current_str[0]);
        return INCORRECT_REGISTR;
    }

    index_of_bytecode_array = assembl->info[number_of_str_in_txt_file].pc + 1;
    assembl->bytecode[index_of_bytecode_array] = current_str[0] - 'A';

    assembl->info[assembl->asm_pc].args = assembl->bytecode[index_of_bytecode_array];
    return NO_MISTAKE_ASM;
}
