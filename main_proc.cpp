#include "parse_asm_from_file.h"
#include "calcul.h"
#include <stdio.h>

int main(){
    asm_code asm_cd = parser_asm("assembler_task/ass.txt");
    printf("res = %d\n", calculate(&asm_cd));
    free(asm_cd.asm_array);
    return 0;
}