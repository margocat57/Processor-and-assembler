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


char* str_add(char* str, CODE_CMD code){
    char* buffer = str + COMANDS_SIZE[code].shift_ptr - 1;
    str = buffer;
    str[0]= code + INT_TO_CHAR;
    return str;
}

char** parse_comands(char** ptr_arr, size_t num_of_str){

    for(size_t idx = 0; idx < num_of_str; idx++){
        if(strstr(ptr_arr[idx], COMANDS_FOR_STRING[PUSH].name_of_comand)){
            ptr_arr[idx] = str_add(ptr_arr[idx], PUSH);
        }
        else if(strstr(ptr_arr[idx], COMANDS_FOR_STRING[ADD].name_of_comand)){
            ptr_arr[idx] = str_add(ptr_arr[idx], ADD);
        }
        else if(strstr(ptr_arr[idx], COMANDS_FOR_STRING[SUB].name_of_comand)){
            ptr_arr[idx] = str_add(ptr_arr[idx], SUB);
        }
        else if(strstr(ptr_arr[idx], COMANDS_FOR_STRING[DIV].name_of_comand)){
            ptr_arr[idx] = str_add(ptr_arr[idx], DIV);
        }
        else if(strstr(ptr_arr[idx], COMANDS_FOR_STRING[MUL].name_of_comand)){
            ptr_arr[idx] = str_add(ptr_arr[idx], MUL);
        }
        else if(strstr(ptr_arr[idx], COMANDS_FOR_STRING[OUT].name_of_comand)){
            ptr_arr[idx] = str_add(ptr_arr[idx], OUT);
        }
        else if(strstr(ptr_arr[idx], COMANDS_FOR_STRING[VLT].name_of_comand)){
            ptr_arr[idx] = str_add(ptr_arr[idx], VLT);
        }
        else if(strstr(ptr_arr[idx], COMANDS_FOR_STRING[SQRT].name_of_comand)){
            ptr_arr[idx] = str_add(ptr_arr[idx], SQRT);
        }
    }

    return ptr_arr;
}