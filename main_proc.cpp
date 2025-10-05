#include "parse_asm_from_file.h"
#include "calcul.h"
#include "processor.h"
#include <stdio.h>

int main(){
    processor intel = {};
    intel = init("assembler_task/ass.txt");
    int a = calculate(&intel);
    printf("res = %d", a);

    processor_free(&intel);
    return 0;
}