#include <stdio.h>
#include "file_work.h"
#include "parsing_str.h"
#include "assembler_struct.h"

int main(){
    assembler assembl = asm_init("assembler_task/calc_fact.txt");
    if (parser(&assembl)){
        free_asm(&assembl);
        return 0;
    }
    asm_dump(&assembl);
    put_buffer_to_file("assembler_task/ass.bin", &assembl);
    free_asm(&assembl);
    return 0;
}
