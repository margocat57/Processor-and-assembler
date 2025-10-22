#include <stdio.h>
#include "assembler_struct.h"
#include "file_work.h"
#include "parsing_str.h"
#include "metki.h"
#include "asm_color_lib.h"

assembler asm_init(const char* instructions_file){
    assembler assembl = {};
    assembl.file_in_arr = read_file_to_string_array(instructions_file);
    if(!assembl.file_in_arr.all_strings_in_file){
        fprintf(stderr, "Can't read info from file to string array");
        return {};
    }

    assembl.ptr_array = create_ptr_array(&assembl.file_in_arr);
    if(!assembl.ptr_array){
        fprintf(stderr, "Can't create ptr array");
        free_asm(&assembl);
        return {};
    }

    assembl.info = fill_listing_struct(&assembl);
    if(!assembl.info){
        fprintf(stderr, "Can't create struct with instucton, bytecode index");
        free_asm(&assembl);
        return {};
    }

    assembl.metki_asm = metki_init(&assembl);
    if(!assembl.metki_asm.metki_arr){
        fprintf(stderr, "Can't create metki array");
        free_asm(&assembl);
        return {};
    }
    
    return assembl;
}


void asm_dump(assembler* assembl){
    if(!assembl || !assembl->info){
        fprintf(stderr, "Can't work - NULL assembler ptr");
        return;
    }

    assembl->asm_pc = 0;
    const char* pc_str       = "PC";
    const char* assembly_str = "ASSEMBLY";
    const char* bytecode_str = "BYTECODE";
    const char* args_str     = "ARGS";
    fprintf(stderr, BLUE_BRIGHT("[%5s] "), pc_str);
    fprintf(stderr, ROYAL_BLUE("%-40s "), assembly_str);
    fprintf(stderr, SKY_BLUE("%8s "), bytecode_str);
    fprintf(stderr, DARK_BLUE("%s\n"), args_str);
    for( ; assembl->asm_pc < assembl->file_in_arr.amount_str; assembl->asm_pc++){
        if(assembl->info[assembl->asm_pc].pc == -1 || !assembl->info[assembl->asm_pc].instruction){
            continue;
        }
        fprintf(stderr, BLUE_BRIGHT("[%5lld] "), assembl->info[assembl->asm_pc].pc);
        fprintf(stderr, ROYAL_BLUE("%-40s "), assembl->info[assembl->asm_pc].instruction);
        fprintf(stderr, SKY_BLUE("%08d "), assembl->info[assembl->asm_pc].bytecode);
        if(assembl->info[assembl->asm_pc].num_of_args >= 1){
            fprintf(stderr, DARK_BLUE("%d"), assembl->info[assembl->asm_pc].args);
        }
        fprintf(stderr, "\n");
    }
}

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
        memset(assembl->info, 0, assembl->file_in_arr.amount_str * sizeof(instruction_info));
        free(assembl->info);
        assembl->info = NULL;
    }
    metki_destroy(assembl);
}