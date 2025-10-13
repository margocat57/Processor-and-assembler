#include "parse_asm_from_file.h"
#include "calcul.h"
#include "processor.h"
#include <stdio.h>

int main(){
    processor intel = {};
<<<<<<< HEAD
    intel = init("assembler_task/ass.bin");
=======
    intel = init("assembler_task/ass.txt");
>>>>>>> origin/main
    stack_err_bytes a = calculate(&intel);
    // printf("res = %d", a.res);

    processor_free(&intel);
    return 0;
}