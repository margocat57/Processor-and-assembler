#include <stdio.h>
#include "file_work.h"
#include "parsing_str.h"

int main(){
    
    file_in_array file_arr = read_file_to_string_array("assembler_task/calc.txt");
    fprintf(stderr, "%p\n", &file_arr.all_strings_in_file);
    char** arr = create_ptr_array(&file_arr);
    fprintf(stderr, "%p\n", arr);
    bytecode code = parser(arr, file_arr.amount_str);
    put_buffer_to_file("assembler_task/ass.bin", &code);
    fprintf(stderr, "%p\n", code.array);

    // TODO to func and memset(0)
    free(file_arr.all_strings_in_file);
    free(arr);
    free(code.array);
    // free(code.array);
    // free_all(&file_arr, &cmnds, arr);

    return 0;
}
// ./calc