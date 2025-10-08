#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "parsing_str.h"

// ключ -о чем сборка в мэйке лучше сборки в одной строке
// сколько стадий в мэйке
// чем собрка в 2 стадии лучше сборки в одну стадию
// зачем -с
// почему нужно подключать флаги на этапе компиляции

// size_t length = strcspn(str, " \t\n\r\f\v");
// проблема - в одном случае возвращает пустую структуру а в других ничего не возвращает
static bool parse_cmnds(size_t* count, int* arr_with_code, char* current_str);

static bool pushr_popr(int cmd, size_t* count, int* arr_with_code, char* current_str);

static void jbe_jae(int cmd, size_t* count, int* arr_with_code, char* current_str);

static void push(int cmd, size_t* count, int* arr_with_code, char* current_str);

bytecode parser(char** ptr_arr, size_t num_of_str){
    bytecode code = {};
    int* arr_with_code = (int*)calloc(num_of_str * 2, sizeof(int));
    size_t count = 0;
    char* current_str = NULL;

    for(size_t idx = 0; idx < num_of_str; idx++, count++){
        current_str = ptr_arr[idx];
        current_str += strspn(current_str, " \t\n\r\f\v");

        if(!parse_cmnds(&count, arr_with_code, current_str)){
            return code;
        }
    }
    code.array = arr_with_code;
    code.size = count;

    // DEBUG_FOR
    for(int i = 0; i < code.size; i++){
        fprintf(stderr, "[%d]: %d\n", i, arr_with_code[i]);
    }

    return code;
}

static bool parse_cmnds(size_t *count, int* arr_with_code, char* current_str){
    size_t length = 0;
    for(size_t cmd = 1; cmd < AMNT_CMD; cmd++){
        length = strcspn(current_str, " \t\n\r\f\v");

        if(length == COMANDS[cmd].size && !strncmp(current_str, COMANDS[cmd].name_of_comand, COMANDS[cmd].size)){
            if(cmd == 17 || cmd == 18){
                if(!pushr_popr(cmd, count, arr_with_code, current_str)){
                    return false;
                }
                break;
            }

            if(cmd == JBE || cmd == JAE){
                jbe_jae(cmd, count, arr_with_code, current_str);
                break;
            }

            if(cmd == PUSH){
                push(cmd, count, arr_with_code, current_str);
                break;
            }

            arr_with_code[*count] = COMANDS[cmd].bytecode;
            break;
        }
    }
    return true;
}

static void push(int cmd, size_t* count, int* arr_with_code, char* current_str){
    arr_with_code[*count] = cmd;
    current_str = current_str + COMANDS[cmd].size + 1;
    arr_with_code[*count + 1] = atoi(current_str);
    (*count)++;
}

static void jbe_jae(int cmd, size_t* count, int* arr_with_code, char* current_str){
    arr_with_code[*count] = cmd;
    (*count)++;

    current_str = current_str + COMANDS[cmd].size + 1; // размер команды
    arr_with_code[*count] = atoi(current_str);
}

static bool pushr_popr(int cmd, size_t* count, int* arr_with_code, char* current_str){
    int cmd_index = cmd == 18 ? PUSHR : POPR;
    arr_with_code[*count] = cmd_index;

    (*count)++;
    current_str = current_str + COMANDS[cmd].size + 1;

    current_str = strchr(current_str, 'X') - 1;
    if ('A' > current_str[0] || current_str[0] > 'P'){
        fprintf(stderr, "Incorrect registr R%cX", current_str[0]);
        return false;
    }

    arr_with_code[*count] = current_str[0] - 'A';
    return true;
}