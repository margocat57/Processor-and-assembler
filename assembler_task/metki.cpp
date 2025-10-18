#include "metki.h"
#include <stdio.h>
#include <stdlib.h>

metki metki_init(assembler* assembl){
    metki m = {};
    int* metki_arr = (int*)calloc(sizeof(int), MAX_NUMBER_OF_METKI); 
    if(!metki_arr){
        fprintf(stderr, "Can't allocate memory for metki array");
        return m;
    }

    char* current_str = NULL;
    int count = 0;
    int metka = 0;

    for(size_t idx = 0; idx < assembl->file_in_arr.amount_str; idx++){
        if(assembl->info[idx].pc == -1){
            // в целом это условие избыточное - можно просто ++ но я решила перестраховаться
            current_str = strchr(assembl->info[idx].instruction, ':') + 1;
            metka = atoi(current_str);
            if(metka >= MAX_NUMBER_OF_METKI){
                fprintf(stderr, "Try to set metka out of array");
                break;
            }
            if(idx + 1 < assembl->file_in_arr.amount_str){
                metki_arr[metka] = assembl->info[idx + 1].pc; 
            }
        }
    }
    m.metki_arr = metki_arr;

    // DEBUG
    for(int i = 0; i < 10; i++){
        printf("[%d]: %d\n", i, metki_arr[i]);
    }
    printf("---------\n");

    return m;
}

void metki_destroy(assembler* assembl){
    if(assembl->metki_asm.metki_arr){
        memset(assembl->metki_asm.metki_arr, 0, MAX_NUMBER_OF_METKI * sizeof(int));
        free(assembl->metki_asm.metki_arr);
        assembl->metki_asm.metki_arr = NULL;
    }
}