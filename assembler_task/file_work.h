#ifndef FILE_WORK_H
#define FILE_WORK_H
#include <stdlib.h>
struct file_in_array
{
    size_t amount_str;         
    char *all_strings_in_file; 
    bool is_stat_err;          
};

bool incorr_work_with_stat(const char *name_of_file, struct stat *all_info_about_file);

int count_strings_by_symbols(char *array_to_search, char ch);

file_in_array read_file_to_string_array(const char *name_of_file);

char **create_ptr_array(file_in_array *arr);

void put_buffer_to_file(const char *name_of_file, const size_t amount_str, char** search_ptr);
#endif //FILE_WORK_H