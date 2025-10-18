#include "assembler_struct.h"
#include "file_work.h"
#include "parsing_str.h"
#include "metki.h"

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
    return assembl;
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