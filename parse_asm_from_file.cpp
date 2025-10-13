#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "string.h"
#include <sys/stat.h>
#include "parse_asm_from_file.h"
#include "assembler_task/parsing_str.h"
#include "processor.h"


code_and_size load_code(const char* name_of_file){
    code_and_size code = {};
    char author_buffer[255] = {};

    FILE *fp = fopen(name_of_file, "rb");
    if(!fp){
        fprintf(stderr, "Can't open file");
        return code;
    }

    fread(&author_buffer, sizeof(char), ELEM_IN_STR, fp);

    printf("%s\n", author_buffer);
    printf("%s\n", BYTECODE_AUTOR_STR);
    
    if (memcmp(BYTECODE_AUTOR_STR, author_buffer, 15)){
        fprintf(stderr, "NOT CORRECT BYTECODE'S AUTHOR OR VERSION");
        return code;
    }

    fread(&code.size, sizeof(size_t), 1, fp);

    int* arr = (int*)calloc((code.size + 1), sizeof(int));
    if(!arr){
        fprintf(stderr, "Can't allocate memory for bytecode array");
        return code;
    }
    int idx = 0;

    fread(arr, sizeof(int), code.size, fp);

    int elem = 0;
    // DEBUG
    for(; elem < code.size; elem++){
        // fscanf(fp, " %d", &arr[elem]);
        fprintf(stderr, "[%d] %d\n", elem, arr[elem]);
    }

    fclose(fp);
    code.comands = arr;

    return code;
}
