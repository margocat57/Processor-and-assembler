#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "string.h"
#include "assembler_task/file_work.h"
#include <sys/stat.h>
#include "parse_asm_from_file.h"

int number_of_tests(const char* name_of_file){
    FILE *fp = fopen(name_of_file, "r");
    char buffer[255] = {}; 

    int count = 0;
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        count++;
    }

    fclose(fp); 
    
    return count - 1;
}

asm_code parser_asm(const char* name_of_file){
    asm_code code = {};
    code.size = number_of_tests(name_of_file);

    FILE *fp = fopen(name_of_file, "r");
    char buffer[255] = {};

    int* arr = (int*)calloc(code.size * 2, sizeof(int));
    int idx = 0;

    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        if(buffer[0] == '1' && (buffer[1] < '0' || buffer[1] > '9')){
            sscanf(buffer, " %d %d", &arr[idx], &arr[idx+1]);
            printf("%d %d\n", arr[idx], arr[idx + 1]);
            idx += 2;
        }
        else{
            sscanf(buffer, " %d ", &arr[idx]);
            printf("%d\n", arr[idx]);
            idx++;
        }
    }
    fclose(fp);
    code.asm_array = arr;
    code.size = idx;
    printf("%d\n", idx);

    int* arr_new = (int*)realloc(arr, idx*sizeof(int));
    if(!arr_new){
        return code;
    }
    arr_new = arr;

    code.asm_array = arr_new;

    return code;
}
