#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "parsing_str.h"


const int INT_TO_CHAR = 49;
// сделать перевод на ассемблерный язык
// программы ассемлер и процессор
// ассемблер переводит в байткод
// процессор исполняет
// проверить что дискриминат нормально работает
// раздельная компиляция с двумя мэйнами
// system assembler 
// system processor


// ключ -о чем сборка в мэйке лучше сборки в одной строке
// сколько стадий в мэйке
// чем собрка в 2 стадии лучше сборки в одну стадию
// зачем -с
// почему нужно подключать флаги на этапе компиляции

void parse_comands(char** ptr_arr, size_t num_of_str, const char* name_of_file){

    FILE *fptr = fopen(name_of_file, "w");
    int temp = 0;
    fprintf(fptr, "%d ", num_of_str);
    fprintf(fptr, "\n");

    for(size_t idx = 0; idx < num_of_str; idx++){
        for(size_t cmd = 0; cmd < AMNT_CMD; cmd++){
            if(!strncmp(ptr_arr[idx], COMANDS[cmd].name_of_comand, COMANDS[cmd].size)){
                
                if(!strncmp(ptr_arr[idx], "PUSHR", 5) || !strncmp(ptr_arr[idx], "POPR", 4)){
                    //TODO убрать говнокод
                    if(!strncmp(ptr_arr[idx], "PUSHR", 5)){
                        fprintf(fptr, "%d ", COMANDS[10].bytecode);
                    }
                    else{
                        fprintf(fptr, "%d ", COMANDS[9].bytecode);
                    }
                    ptr_arr[idx] = strchr(ptr_arr[idx], 'X') - 1;
                    if('A' <= ptr_arr[idx][0] && ptr_arr[idx][0] <= 'P')
                        fprintf(fptr, "%d \n", ptr_arr[idx][0] - 'A');
                    else
                        exit(EXIT_FAILURE);
                    break;
                }

                if(!strncmp(ptr_arr[idx], "PUSH", 4) && strncmp(ptr_arr[idx], "PUSHR", 5)){
                    fprintf(fptr, "%d ", COMANDS[cmd].bytecode);
                    ptr_arr[idx] = ptr_arr[idx] + COMANDS[cmd].size;
                    fprintf(fptr, "%d \n", atoi(ptr_arr[idx]));
                    break;
                }

                fprintf(fptr, "%d \n", COMANDS[cmd].bytecode);
                break;
            }
        }
    }

}


/*
bytecode parse_comands(char** ptr_arr, size_t num_of_str){

    bytecode code = {};
    int* arr_with_code = (int*)calloc(num_of_str * 2, sizeof(int));
    size_t count = 0;

    // TODO strtol
    //TODO check correct output
    for(size_t idx = 0; idx < num_of_str; idx++, count++){
        for(size_t cmd = 0; cmd < AMNT_CMD; cmd++){
            if(strncmp(ptr_arr[idx], COMANDS[cmd].name_of_comand, COMANDS[cmd].size)){
                if(cmd == 0){
                    arr_with_code[count] = PUSH;
                    ptr_arr[idx] = ptr_arr[idx] + COMANDS[cmd].size;
                    arr_with_code[count + 1] = atoi(ptr_arr[idx]);
                    count++;
                }
                
                arr_with_code[count] = COMANDS[cmd].bytecode;
                break;
            }
        }
    }

    // TODO check realloc
    arr_with_code = (int*)realloc(arr_with_code, sizeof(int)*count);
    code.array = arr_with_code;
    code.size = count;

    return code;
}
*/