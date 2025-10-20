#include <stdlib.h>
#include <sys/stat.h>
#include <assert.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include "file_work.h"
#include "parsing_str.h"

bool incorr_work_with_stat(const char *name_of_file, struct stat *all_info_about_file)
{
    if(!name_of_file){
        fprintf(stderr, "NULL pointer to file name");
        return true;
    }
    if(!all_info_about_file){
        fprintf(stderr, "NULL pointer to struct of all file info");
        return true;
    }

    if (stat(name_of_file, all_info_about_file) == -1)
    {
        perror("Stat error");
        fprintf(stderr, "Error code: %d\n", errno);
        return true;
    }
    return false;
}

int count_strings_by_symbols(char *array_to_search, char ch)
{
    if(!array_to_search){
        fprintf(stderr, "NULL pointer to array_to_search");
        return 0;
    }

    int count_str = 1; // 1

    while ((array_to_search = strchr(array_to_search, ch)) != NULL)
    {
        count_str++;
        *(array_to_search) = '\0';
        array_to_search++;
    }

    return count_str;
}

file_in_array read_file_to_string_array(const char *name_of_file){   
    file_in_array fptr_in_array = {};
    if(!name_of_file){
        fprintf(stderr, "NULL pointer to file name");
        return fptr_in_array;
    }
    FILE *fptr = fopen(name_of_file, "r");
    if(!fptr){
        fprintf(stderr, "Can't open file");
        return fptr_in_array;
    }

    struct stat file_info = {};
    fptr_in_array.is_stat_err = incorr_work_with_stat(name_of_file, &(file_info));
    if (fptr_in_array.is_stat_err)
    {
        return fptr_in_array;
    }

    char *all_strings_in_file = (char *)calloc(file_info.st_size + 1, sizeof(char));
    if(!all_strings_in_file){
        fprintf(stderr, "ALLOC ERROR for all strings array");
        return fptr_in_array;
    }

    if(fread(all_strings_in_file, sizeof(char), file_info.st_size, fptr) != file_info.st_size){
        fprintf(stderr, "Can't read all strings from file to buffer");
        return fptr_in_array;
    }

    char *search_ptr = all_strings_in_file;

    size_t nmb_of_str = count_strings_by_symbols(search_ptr, '\n');

    fptr_in_array.amount_str = nmb_of_str;
    fptr_in_array.all_strings_in_file = all_strings_in_file;

    fclose(fptr);
    return fptr_in_array;
}

char** create_ptr_array(file_in_array* file_in_arr){   
    if(!file_in_arr->all_strings_in_file ){
        fprintf(stderr, "NULL pointer to all strings array");
        return NULL;
    }

    char** arr_with_ptr_sz = (char **)calloc(file_in_arr->amount_str, sizeof(char*));
    if(!arr_with_ptr_sz){
        fprintf(stderr, "ALLOC ERROR for ptr_array");
        return NULL;
    }

    size_t num_of_elem = 0;
    arr_with_ptr_sz[num_of_elem] = file_in_arr->all_strings_in_file;
    num_of_elem = 1;

    char *search_ptr = file_in_arr->all_strings_in_file;

    for (num_of_elem = 1; num_of_elem <= file_in_arr->amount_str; num_of_elem++)
    {
        arr_with_ptr_sz[num_of_elem] = search_ptr + strlen(search_ptr) + 1;
        if (num_of_elem != file_in_arr->amount_str)
            search_ptr += strlen(search_ptr) + 1;
    }

    return arr_with_ptr_sz;
}

// если это массив строк то можно сделать переносы
void put_buffer_to_file(const char *name_of_file, assembler* assembl)
{
    if(!name_of_file){
        fprintf(stderr, "NULL pointer to file name - can't work");
        return;
    }
    if(!assembl){
        fprintf(stderr, "NULL pointer to asm struct - can't work");
        return;
    }

    FILE *fptr = fopen(name_of_file, "wb");
    if(!fptr){
        fprintf(stderr, "Can't open file to write");
        return;
    }

    if(BYTECODE_AUTOR_STR){
        if(fwrite(BYTECODE_AUTOR_STR, sizeof(char), strlen(BYTECODE_AUTOR_STR), fptr) != strlen(BYTECODE_AUTOR_STR)){
            fprintf(stderr, "Can't write author str to file - writing stopped");
            fclose(fptr);
            return;
        }
    }

    if(assembl->asm_bytecode_size){
        if(fwrite(&assembl->asm_bytecode_size, sizeof(size_t), 1, fptr) != 1){
            fprintf(stderr, "Can't write num of elem of bytecode to file - writing stopped");
            fclose(fptr);
            return;
        }
    }

    if(assembl->bytecode){
        if(fwrite(assembl->bytecode, sizeof(int), assembl->asm_bytecode_size, fptr) != assembl->asm_bytecode_size){
            fprintf(stderr, "Can't write num of elem of bytecode to file - writing stopped");
            fclose(fptr);
            return;
        }
    }

    fclose(fptr);
}

