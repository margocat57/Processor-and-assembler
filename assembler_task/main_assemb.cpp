#include <stdio.h>
#include "file_work.h"
#include "parsing_str.h"
#include "assembler_struct.h"
#include "../stack_for_calcul/hash.h"

int main(){
    fprintf(stderr, "HLT = %zu", create_djb2_hash("HLT", 3));
    assembler assembl = asm_init("assembler_task/mem_test.txt");
    if (parser(&assembl)){
        free_asm(&assembl);
        return 0;
    }
    asm_dump(&assembl);
    put_buffer_to_file("assembler_task/ass.bin", &assembl);
    free_asm(&assembl);
    return 0;
}
