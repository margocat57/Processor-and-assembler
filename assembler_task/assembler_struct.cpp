#include <stdio.h>
#include "assembler_struct.h"
#include "file_work.h"
#include "parsing_str.h"
#include "metki.h"
#include "asm_color_lib.h"

assembler asm_init(const char* instructions_file){
    assembler assembl = {};
    // проверка что аргументы не пустые
    assembl.file_in_arr = read_file_to_string_array(instructions_file);
    assembl.ptr_array = create_ptr_array(&assembl.file_in_arr);
    assembl.info = fill_listing_struct(&assembl);
    assembl.metki_asm = metki_init(&assembl);
    if (parser(&assembl)){
        free_asm(&assembl);
        return {};
    }
    asm_dump(&assembl);
    return assembl;
}


void asm_dump(assembler* assembl){
    assembl->asm_pc = 0;
    const char* pc_str       = "PC";
    const char* assembly_str = "ASSEMBLY";
    const char* bytecode_str = "BYTECODE";
    const char* args_str     = "ARGS";
    fprintf(stderr, BLUE_BRIGHT("[%5s] "), pc_str);
    fprintf(stderr, ROYAL_BLUE("%-25s "), assembly_str);
    fprintf(stderr, SKY_BLUE("%8s "), bytecode_str);
    fprintf(stderr, DARK_BLUE("%s\n"), args_str);
    for( ; assembl->asm_pc < assembl->file_in_arr.amount_str; assembl->asm_pc++){
        if(assembl->info[assembl->asm_pc].pc == -1 || !assembl->info[assembl->asm_pc].instruction){
            continue;
        }
        fprintf(stderr, BLUE_BRIGHT("[%5lld] "), assembl->info[assembl->asm_pc].pc);
        fprintf(stderr, ROYAL_BLUE("%-25s "), assembl->info[assembl->asm_pc].instruction);
        fprintf(stderr, SKY_BLUE("%08d "), assembl->info[assembl->asm_pc].bytecode);
        if(assembl->info[assembl->asm_pc].num_of_args >= 1){
            fprintf(stderr, DARK_BLUE("%d"), assembl->info[assembl->asm_pc].args);
        }
        fprintf(stderr, "\n");
    }
}

//верификатор ассемблера

void free_asm(assembler* assembl){
    if(assembl->file_in_arr.all_strings_in_file){
        memset(assembl->file_in_arr.all_strings_in_file, 0, assembl->file_in_arr.amount_str * sizeof(char));
        free(assembl->file_in_arr.all_strings_in_file);
        assembl->file_in_arr.all_strings_in_file = NULL;
    }

    if(assembl->bytecode){
        memset(assembl->bytecode, 0, assembl->asm_bytecode_size * sizeof(int));
        free(assembl->bytecode);
        assembl->bytecode = NULL;
    }

    if(assembl->ptr_array){
        memset(assembl->ptr_array, 0, assembl->file_in_arr.amount_str * sizeof(char*));
        free(assembl->ptr_array);
        assembl->ptr_array = NULL;
    }

    if(assembl->info){
        memset(assembl->info, 0, assembl->file_in_arr.amount_str * sizeof(listing));
        free(assembl->info);
        assembl->info = NULL;
    }
    metki_destroy(assembl);
}