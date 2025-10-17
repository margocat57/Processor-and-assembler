#include <stdio.h>
#include "file_work.h"
#include "parsing_str.h"

int main(){
    
    assembler asssembl = read_file_to_string_array("assembler_task/calc_fact.txt");
    create_ptr_array(&asssembl); // TODO: asm_init 

    parser(&asssembl);
    put_buffer_to_file("assembler_task/ass.bin", &asssembl);

    // TODO to func and memset(0)
    free_all(&asssembl);
    // free(code.array);
    // free(code.array);
    // free_all(&file_arr, &code, arr);

    return 0;
}
// ./calc