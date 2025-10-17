#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "parsing_str.h"

// стоит добавить метки как в ассемблере

// проблема - в одном случае возвращает пустую структуру а в других ничего не возвращает
static bool parse_cmnds(size_t* count, int* arr_with_code, char* current_str, int* metki_arr);

static bool pushr_popr(int cmd, size_t* count, int* arr_with_code, char* current_str);

static bool pushm_popm(int cmd, size_t* count, int* arr_with_code, char* current_str);

static void func_with_metka(int cmd, size_t* count, int* arr_with_code, char* current_str, int* metki_arr);

static void push(int cmd, size_t* count, int* arr_with_code, char* current_str);

static int* metki(assembler* assembl);

static int* metki(assembler* assembl){
    int* metki_arr = (int*)calloc(sizeof(int), MAX_NUMBER_OF_METKI); 
    if(!metki_arr){
        fprintf(stderr, "Can't allocate memory for metki array");
        return NULL;
    }

    char* current_str = NULL;
    int count = 0;
    int metka = 0;

    for(size_t idx = 0; idx < assembl->file_in_arr.amount_str; idx++){
        printf("%d\n", count);
        //FIXME не нравится, долго
        for(size_t cmd = 1; cmd < AMNT_CMD; cmd++){
            assembl->ptr_array[idx] += strspn(assembl->ptr_array[idx], " \t\n\r\f\v");
            if((cmd == 21 || cmd == 22 || cmd == JBE 
            || cmd == JAE || cmd == JE || cmd == JNE 
            || cmd == JA  || cmd == JB || cmd == PUSH 
            || cmd == CALL || cmd == PUSHM || cmd == POPM) &&
            !strncmp(assembl->ptr_array[idx], COMANDS[cmd].name_of_comand, COMANDS[cmd].size)){
                count++;
                break;
            }
        }
        // можно сделать так что если типо команды перед меткой нет только тогда мы изменяем значения массива меток

        current_str = strchr(assembl->ptr_array[idx], ':');
        // зачем такое условное условие затем чтобы если метка была до команды JUMP :метка мы тоже прыгали по метке
        if(current_str && (current_str == assembl->ptr_array[idx] + strspn(assembl->ptr_array[idx], " \t\n\r\f\v"))){
            current_str++;
            metka = atoi(current_str);
            metki_arr[metka] = count;   //- 1;
            continue;
        }
        count++;
    }

    // DEBUG
    for(int i = 0; i < 10; i++){
        printf("[%d]: %d\n", i, metki_arr[i]);
    }
    printf("---------\n");

    return metki_arr;
}

assembler* parser(assembler* assembl){
    bytecode code = {};

    int* metki_arr = metki(assembl);
    if(!metki_arr){
        fprintf(stderr, "Can't allocate memory for metki array");
        return assembl;
    }

    int* arr_with_code = (int*)calloc(assembl->file_in_arr.amount_str * 2, sizeof(int));
    if(!metki_arr){
        fprintf(stderr, "Can't allocate memory for bytecode array");
        return assembl;
    }

    size_t count = 0;
    char* current_str = NULL;
    fprintf(stderr, "%d\n", assembl->file_in_arr.amount_str);

    for(size_t idx = 0; idx < assembl->file_in_arr.amount_str; idx++){
        current_str = assembl->ptr_array[idx];
        if(!current_str){
            fprintf(stderr, "current ptr is null, parsing stopped");
            break;
        }

        current_str += strspn(current_str, " \t\n\r\f\v");

        if(!parse_cmnds(&count, arr_with_code, current_str, metki_arr)){
            return assembl;
        }
        fprintf(stderr, "%s\n", current_str);
        fprintf(stderr, "%d\n", count);
    }
    code.array = arr_with_code;
    code.size = count;
    fprintf(stderr, "%d\n", count);
    memset(metki_arr, 0, MAX_NUMBER_OF_METKI * sizeof(int));
    free(metki_arr);

    // DEBUG_FOR
    for(int i = 0; i < code.size; i++){
        fprintf(stderr, "[%d]: %d\n", i, arr_with_code[i]);
    }

    assembl->bytecode_struct = code;

    return assembl;
}

static bool parse_cmnds(size_t *count, int* arr_with_code, char* current_str, int* metki_arr){
    size_t length = 0;
    bool command_found = false;
    for(size_t cmd = 1; cmd < AMNT_CMD; cmd++){
        length = strcspn(current_str, " \t\n\r\f\v");

        if(length == COMANDS[cmd].size && !strncmp(current_str, COMANDS[cmd].name_of_comand, COMANDS[cmd].size)){
            if(cmd == 21 || cmd == 22){
                if(!pushr_popr(cmd, count, arr_with_code, current_str)){
                    return false;
                }
                command_found = true;
                break;
            }

            if(cmd == PUSHM || cmd == POPM){
                if(!pushm_popm(cmd, count, arr_with_code, current_str)){
                    return false;
                }
                command_found = true;
                break;
            }

            if(cmd == JBE || cmd == JAE || cmd == JE || cmd == JNE || cmd == JA || cmd == JB || cmd == CALL || cmd == JMP){
                func_with_metka(cmd, count, arr_with_code, current_str, metki_arr);
                command_found = true;
                break;
            }

            if(cmd == PUSH){
                push(cmd, count, arr_with_code, current_str);
                command_found = true;
                break;
            }

            arr_with_code[*count] = COMANDS[cmd].bytecode;
            command_found = true;
            break;
        }
    }
    fprintf(stderr, "command_found = %d\n", command_found);
    if(command_found) (*count)++;
    return true;
}

static void push(int cmd, size_t* count, int* arr_with_code, char* current_str){
    arr_with_code[*count] = cmd;
    current_str = current_str + COMANDS[cmd].size + 1;
    arr_with_code[*count + 1] = atoi(current_str);
    (*count)++;
}

static void func_with_metka(int cmd, size_t* count, int* arr_with_code, char* current_str, int* metki_arr){
    arr_with_code[*count] = cmd;
    (*count)++;

    current_str = strchr(current_str, ':'); // доходим до метки
    current_str++; // доходим до числа

    arr_with_code[*count] = metki_arr[atoi(current_str)];
}

static bool pushr_popr(int cmd, size_t* count, int* arr_with_code, char* current_str){
    int cmd_index = cmd == 22 ? PUSHR : POPR;
    arr_with_code[*count] = cmd_index;

    (*count)++;
    current_str = current_str + COMANDS[cmd].size + 1;

    current_str = strchr(current_str, 'X') - 1;
    if ('A' > current_str[0] || current_str[0] > 'P'){
        fprintf(stderr, "Incorrect registr R%cXin parsing pushr/popr cmd", current_str[0]);
        return false;
    }

    arr_with_code[*count] = current_str[0] - 'A';
    return true;
}

static bool pushm_popm(int cmd, size_t* count, int* arr_with_code, char* current_str){
    arr_with_code[*count] = cmd;

    (*count)++;
    current_str = current_str + COMANDS[cmd].size + 1;

    current_str = strchr(current_str, 'X') - 1;
    if ('A' > current_str[0] || current_str[0] > 'P'){
        fprintf(stderr, "Incorrect registr R%cX in parsing pushm/popm cmd", current_str[0]);
        return false;
    }

    arr_with_code[*count] = current_str[0] - 'A';
    return true;
}