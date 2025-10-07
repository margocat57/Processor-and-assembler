#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "parsing_str.h"

// ключ -о чем сборка в мэйке лучше сборки в одной строке
// сколько стадий в мэйке
// чем собрка в 2 стадии лучше сборки в одну стадию
// зачем -с
// почему нужно подключать флаги на этапе компиляции

// TODO switch easyier
// и вообще как будто здесь нужно создавать массив int его оьрабатывать и затем класть в файл
void parse_comands(char** ptr_arr, size_t num_of_str, const char* name_of_file){

    FILE *fptr = fopen(name_of_file, "w");
    fprintf(fptr, BYTECODE_AUTOR_STR);
    fprintf(fptr, "%d ", num_of_str);
    fprintf(fptr, "\n");

    for(size_t idx = 0; idx < num_of_str; idx++){
        ptr_arr[idx] += strspn(ptr_arr[idx], " \t\n\r\f\v");
        // fprintf(stderr, "%s\n", ptr_arr[idx]);
        // fprintf(stderr, "!strncmp(ptr_arr[idx], JBE, 3) = %d\n", !strncmp(ptr_arr[idx], "JBE", 3));
        for(size_t cmd = 0; cmd < AMNT_CMD; cmd++){
            if(!strncmp(ptr_arr[idx], COMANDS[cmd].name_of_comand, COMANDS[cmd].size)){
                if (!strncmp(ptr_arr[idx], "JBE", 3) || !strncmp(ptr_arr[idx], "JAE", 3)) {
                    int cmd_index = !strncmp(ptr_arr[idx], "JBE", 3) ? 12 : 14;
                    fprintf(fptr, "%d ", COMANDS[cmd_index].bytecode);

                    ptr_arr[idx] = ptr_arr[idx] + 3; // размер команды

                    fprintf(fptr, "%d \n", atoi(ptr_arr[idx]));
                    break;
                }

                if (!strncmp(ptr_arr[idx], "PUSHR", 5) || !strncmp(ptr_arr[idx], "POPR", 4)) {
                    int cmd_index = !strncmp(ptr_arr[idx], "PUSHR", 5) ? 10 : 9;
                    fprintf(fptr, "%d ", COMANDS[cmd_index].bytecode);

                    ptr_arr[idx] = strchr(ptr_arr[idx], 'X') - 1;
                    if ('A' > ptr_arr[idx][0] || ptr_arr[idx][0] > 'P'){
                        fprintf(stderr, "Incorrect registr R%cX", ptr_arr[idx][0]);
                        return;
                    }

                    fprintf(fptr, "%d \n", ptr_arr[idx][0] - 'A');
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

bytecode parser(char** ptr_arr, size_t num_of_str){
    bytecode code = {};
    int* arr_with_code = (int*)calloc(num_of_str * 2, sizeof(int));
    size_t count = 0;
    for(size_t idx = 0; idx < num_of_str; idx++, count++){
        for(size_t cmd = 0; cmd < AMNT_CMD; cmd++){
            if(!strncmp(ptr_arr[idx], COMANDS[cmd].name_of_comand, COMANDS[cmd].size)){
                if (!strncmp(ptr_arr[idx], "JBE", 3) || !strncmp(ptr_arr[idx], "JAE", 3)) {
                    int cmd_index = !strncmp(ptr_arr[idx], "JBE", 3) ? JBE : JAE;
                    arr_with_code[count] = cmd_index;
                    count++;

                    ptr_arr[idx] = ptr_arr[idx] + 3; // размер команды

                    arr_with_code[count] = atoi(ptr_arr[idx]);
                    break;
                }

                if (!strncmp(ptr_arr[idx], "PUSHR", 5) || !strncmp(ptr_arr[idx], "POPR", 4)) {
                    int cmd_index = !strncmp(ptr_arr[idx], "PUSHR", 5) ? PUSHR : POPR;
                    arr_with_code[count] = cmd_index;
                    count++;

                    ptr_arr[idx] = strchr(ptr_arr[idx], 'X') - 1;
                    if ('A' > ptr_arr[idx][0] || ptr_arr[idx][0] > 'P'){
                        fprintf(stderr, "Incorrect registr R%cX", ptr_arr[idx][0]);
                        return code;
                    }

                    arr_with_code[count] = ptr_arr[idx][0] - 'A';
                    break;
                }

                if(!strncmp(ptr_arr[idx], "PUSH", 4) && strncmp(ptr_arr[idx], "PUSHR", 5)){
                    arr_with_code[count] = PUSH;
                    ptr_arr[idx] = ptr_arr[idx] + COMANDS[cmd].size;
                    arr_with_code[count + 1] = atoi(ptr_arr[idx]);
                    count++;
                    break;
                }

                arr_with_code[count] = COMANDS[cmd].bytecode;
                break;
            }
        }
    }
    code.array = arr_with_code;
    code.size = count;
    return code;
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