#ifndef FILE_WORK_H
#define FILE_WORK_H
#include <stdlib.h>
#include "parsing_str.h"
#include "assembler_struct.h"

bool incorr_work_with_stat(const char *name_of_file, struct stat *all_info_about_file);

int count_strings_by_symbols(char *array_to_search, char ch);

assembler read_file_to_string_array(const char *name_of_file);

assembler* create_ptr_array(assembler* assembl);

void put_buffer_to_file(const char *name_of_file, assembler* assembl);

void free_all(assembler* assembl);
#endif //FILE_WORK_H