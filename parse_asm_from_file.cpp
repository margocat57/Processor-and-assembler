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

    // убрать константу
    fread(&author_buffer, sizeof(char), 15, fp);
    printf("%s\n", author_buffer);
    
    /*
    unsigned char buffer[100] = {};
    size_t read = fread(buffer, 1, 100, fp);
    
    printf("First 100 bytes of file:\n");
    for (size_t i = 0; i < read; i++) {
        printf("%02x ", buffer[i]);
        if ((i + 1) % 16 == 0) printf("\n");
    }
    printf("\n");
    */


    // fscanf(fp, "%s", &author_buffer);
    /*
    if (!strcmp(author_buffer, BYTECODE_AUTOR_STR)){
        fprintf(stderr, "NOT CORRECT BYTECODE'S AUTHOR OR VERSION");
        return code;
    }
    */

    // оказалось проблемой переменной сайз т
    fread(&code.size, sizeof(size_t), 1, fp);
    printf("%lu\n", code.size);
    code.size = 24;
    // fscanf(fp, " %d", &code.size);
    // code.size *= 2;
    printf("%lu\n", sizeof(size_t));

    int* arr = (int*)calloc((code.size + 1), sizeof(int));
    assert(arr != NULL);
    int idx = 0;

    // TODO fread
    // char* buffer = (char*)calloc(1, sizeof(int) * (code.size - 1));
    // fprintf(stderr, "%d", fread(buffer, sizeof(char), code.size, fp)); - не работает
    fread(arr, sizeof(int), code.size, fp);
    fprintf(stderr, "ngvh");
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
