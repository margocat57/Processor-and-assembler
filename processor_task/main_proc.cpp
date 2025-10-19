#include "parse_asm_from_file.h"
#include "calcul.h"
#include "processor.h"
#include <stdio.h>

int main(){
    processor baikal = {};
    baikal = init("assembler_task/ass.bin");
    stack_err_bytes a = do_processor_comands(&baikal);
    // printf("res = %d", a.res);

    processor_free(&baikal);
    return 0;
}