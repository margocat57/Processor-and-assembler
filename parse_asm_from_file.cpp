#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "string.h"
#include <sys/stat.h>
#include "parse_asm_from_file.h"
#include "processor.h"


code_and_size load_code(const char* name_of_file){
    code_and_size code = {};
    char size = {};

    FILE *fp = fopen(name_of_file, "r");
    fscanf(fp, "%d", &code.size);
    code.size *= 2;

    int* arr = (int*)calloc((code.size - 1) * 2, sizeof(int));
    int idx = 0;

    // TODO fread
    // TODO change code size - может и менять не надо память то дешевая
    // char* buffer = (char*)calloc(1, sizeof(int) * (code.size - 1));
    // fprintf(stderr, "%d", fread(buffer, sizeof(char), code.size, fp)); - не работает

    int i = 0;
    for(; i < code.size; i++){
        fscanf(fp, "%d", &arr[i]);
        fprintf(stderr, "[%d] %d\n", i, arr[i]);
    }

    fclose(fp);
    code.size = i;
    code.comands = arr;

    return code;
}
