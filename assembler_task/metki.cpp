#include "metki.h"
#include "assembler_struct.h"
#include "asm_color_lib.h"
#include <stdio.h>
#include <stdlib.h>

static void metki_dump(metki metki){
    const char* index = "METKA";
    const char* pointer = "Pointer";
    fprintf(stderr, GOLDEN_YELLOW("[%5s] "), index);
    fprintf(stderr, LIGHT_YELLOW("%s\n"), pointer);
    for(int idx = 0; idx < MAX_NUMBER_OF_METKI; idx++){
        fprintf(stderr, GOLDEN_YELLOW("[%5d] "), idx);
        fprintf(stderr, LIGHT_YELLOW("%d\n"), metki.metki_arr[idx]);
    }
}

metki metki_init(assembler* assembl){
    metki mtk = {};
    if(!assembl){
        fprintf(stderr, "Can't work - NULL assembler ptr");
        return mtk;
    }

    int* metki_arr = (int*)calloc(sizeof(int), MAX_NUMBER_OF_METKI); 
    if(!metki_arr){
        fprintf(stderr, "Can't allocate memory for metki array");
        return mtk;
    }

    char* current_str = NULL;
    int metka = 0;

    for(size_t idx = 0; idx < assembl->file_in_arr.amount_str; idx++){
        if(assembl->info[idx].pc == -1){
            current_str = strchr(assembl->info[idx].instruction, ':') + 1;
            metka = atoi(current_str);
            if(metka >= MAX_NUMBER_OF_METKI){
                fprintf(stderr, "Try to set metka out of array");
                break;
            }
            if(idx + 1 < assembl->file_in_arr.amount_str){
                if(metka < 10)
                    metki_arr[metka] = assembl->info[idx + 1].pc; 
                else 
                    metki_arr[0] = assembl->info[idx + 1].pc; 
            }
        }
    }
    mtk.metki_arr = metki_arr;

    metki_dump(mtk);

    return mtk;
}

void metki_destroy(assembler* assembl){
    if(!assembl){
        fprintf(stderr, "Can't work - NULL assembler ptr");
        return;
    }

    if(assembl->metki_asm.metki_arr){
        memset(assembl->metki_asm.metki_arr, 0, MAX_NUMBER_OF_METKI * sizeof(int));
        free(assembl->metki_asm.metki_arr);
        assembl->metki_asm.metki_arr = NULL;
    }
}