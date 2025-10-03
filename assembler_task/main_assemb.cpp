#include <stdio.h>
#include "file_work.h"
#include "parsing_str.h"

int main(){
    
    file_in_array file_arr = read_file_to_string_array("assembler_task/calc.txt");
    char** arr = create_ptr_array(&file_arr);
    char** ar = parse_comands(arr, file_arr.amount_str);
    put_buffer_to_file("assembler_task/ass.txt", file_arr.amount_str, ar);
    // int res = calculate(comands);
    // printf("res = %d", res);
    //free(comands);
    free(file_arr.all_strings_in_file);
    free(arr);

    return 0;
}
// ./calc