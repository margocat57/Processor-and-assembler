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
    fscanf(fp, "%s", &author_buffer);
    if (!strcmp(author_buffer, BYTECODE_AUTOR_STR)){
        fprintf(stderr, "NOT CORRECT BYTECODE'S AUTHOR OR VERSION");
        return code;
    }
    fscanf(fp, " %d", &code.size);
    // code.size *= 2;

    int* arr = (int*)calloc((code.size), sizeof(int));
    int idx = 0;

    // TODO fread
    // char* buffer = (char*)calloc(1, sizeof(int) * (code.size - 1));
    // fprintf(stderr, "%d", fread(buffer, sizeof(char), code.size, fp)); - не работает
    fread(arr, sizeof(int), code.size, fp);
    int elem = 0;
    for(; elem < code.size; elem++){
        // fscanf(fp, " %d", &arr[elem]);
        fprintf(stderr, "[%d] %d\n", elem, arr[elem]);
    }

    fclose(fp);
    code.size = elem;
    code.comands = arr;

    return code;
}
