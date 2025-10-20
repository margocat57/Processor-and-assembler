#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "string.h"
#include <sys/stat.h>
#include "parse_asm_from_file.h"
#include "../assembler_task/parsing_str.h"
#include "processor.h"


code_and_size load_code(const char* name_of_file){
    code_and_size code = {};
    char author_buffer[255] = {};

    FILE *fp = fopen(name_of_file, "rb");
    if(!fp){
        fprintf(stderr, "Can't open file");
        return code;
    }

    if(fread(&author_buffer, sizeof(char), ELEM_IN_STR, fp) != ELEM_IN_STR){
        fprintf(stderr, "Can't read bytecode's sign");
        fclose(fp);
        return code;
    }
    
    if (memcmp(BYTECODE_AUTOR_STR, author_buffer, ELEM_IN_STR)){
        fprintf(stderr, "NOT CORRECT BYTECODE'S AUTHOR OR VERSION\n");
        fclose(fp);
        return code;
    }

    if(fread(&code.size, sizeof(size_t), 1, fp) != 1){
        fprintf(stderr, "Can't read number of symbols in bytecode\n");
        fclose(fp);
        return code;
    }

    int* arr = (int*)calloc((code.size + 1), sizeof(int));
    if(!arr){
        fprintf(stderr, "Can't allocate memory for bytecode array\n");
        fclose(fp);
        return code;
    }

    if(fread(arr, sizeof(int), code.size, fp) != code.size){
        fprintf(stderr, "Can't read bytecode from file\n");
        fclose(fp);
        return code;
    }

    fclose(fp);
    code.comands = arr;

    return code;
}
