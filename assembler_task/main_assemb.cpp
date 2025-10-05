#include <stdio.h>
#include "file_work.h"
#include "parsing_str.h"

int main(){
    
    file_in_array file_arr = read_file_to_string_array("assembler_task/calc.txt");
    char** arr = create_ptr_array(&file_arr);
    //comands_and_num_of_str cmnds = {};
    parse_comands(arr, file_arr.amount_str, "assembler_task/ass.txt");
    free(file_arr.all_strings_in_file);
    free(arr);
    // free_all(&file_arr, &cmnds, arr);

    return 0;
}
// ./calc