#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "parsing_str.h"

// стоит добавить метки как в ассемблере

// size_t length = strcspn(str, " \t\n\r\f\v");
// проблема - в одном случае возвращает пустую структуру а в других ничего не возвращает
static bool parse_cmnds(size_t* count, int* arr_with_code, char* current_str, int* metki_arr);

static bool pushr_popr(int cmd, size_t* count, int* arr_with_code, char* current_str);

static void func_with_metka(int cmd, size_t* count, int* arr_with_code, char* current_str, int* metki_arr);

static void push(int cmd, size_t* count, int* arr_with_code, char* current_str);

static int* metki(char** ptr_arr, size_t num_of_str);

static int* metki(char** ptr_arr, size_t num_of_str){
    int* metki_arr = (int*)calloc(sizeof(int), 10); // динамически
    if(!metki_arr){
        fprintf(stderr, "Can't allocate memory for metki array");
        return NULL;
    }

    char* current_str = NULL;
    int count = 0;
    int metka = 0;

    for(size_t idx = 0; idx < num_of_str; idx++){
        printf("%d\n", count);
        //FIXME не нравится, долго
        for(size_t cmd = 1; cmd < AMNT_CMD; cmd++){
            ptr_arr[idx] += strspn(ptr_arr[idx], " \t\n\r\f\v");
            if((cmd == 21 || cmd == 22 || cmd == JBE 
            || cmd == JAE || cmd == JE || cmd == JNE 
            || cmd == JA  || cmd == JB || cmd == PUSH 
            || cmd == CALL || cmd == PUSHM || cmd == POPM) &&
            !strncmp(ptr_arr[idx], COMANDS[cmd].name_of_comand, COMANDS[cmd].size)){
                count++;
                break;
            }
        }
        // можно сделать так что если типо команды перед меткой нет только тогда мы изменяем значения массива меток

        current_str = strchr(ptr_arr[idx], ':');
        // зачем такое условное условие затем чтобы если метка была до команды JUMP :метка мы тоже прыгали по метке
        if(current_str && (current_str == ptr_arr[idx] + strspn(ptr_arr[idx], " \t\n\r\f\v"))){
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

bytecode parser(char** ptr_arr, size_t num_of_str){
    bytecode code = {};

    int* metki_arr = metki(ptr_arr, num_of_str);
    if(!metki_arr){
        fprintf(stderr, "Can't allocate memory for metki array");
        return code;
    }

    int* arr_with_code = (int*)calloc(num_of_str * 2, sizeof(int));
    if(!metki_arr){
        fprintf(stderr, "Can't allocate memory for bytecode array");
        return code;
    }

    size_t count = 0;
    char* current_str = NULL;
    fprintf(stderr, "%d\n", num_of_str);

    for(size_t idx = 0; idx < num_of_str; idx++){
        current_str = ptr_arr[idx];
        if(!current_str){
            fprintf(stderr, "current ptr is null, parsing stopped");
            break;
        }

        current_str += strspn(current_str, " \t\n\r\f\v");

        // fprintf(stderr, "%d\n", idx);
        // fprintf(stderr, "%s\n", current_str);

        if(!parse_cmnds(&count, arr_with_code, current_str, metki_arr)){
            return code;
        }
    }
    code.array = arr_with_code;
    code.size = count;
    free(metki_arr);
    // TODO заполнять нулями

    // DEBUG_FOR
    for(int i = 0; i < code.size; i++){
        fprintf(stderr, "[%d]: %d\n", i, arr_with_code[i]);
    }

    return code;
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
        fprintf(stderr, "Incorrect registr R%cX", current_str[0]);
        return false;
    }

    arr_with_code[*count] = current_str[0] - 'A';
    return true;
}

// TODO здесь более сложная логика так как 100 элементов
// nbgj
static bool pushm_popm(int cmd, size_t* count, int* arr_with_code, char* current_str){
    arr_with_code[*count] = cmd;

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