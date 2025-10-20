#include <stdio.h>
#include "file_work.h"
#include "parsing_str.h"
#include "assembler_struct.h"

int main(){
    assembler asssembl = asm_init("assembler_task/calc_fact.txt");
    put_buffer_to_file("assembler_task/ass.bin", &asssembl);
    free_asm(&asssembl);
    return 0;
}
