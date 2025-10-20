#include "parse_asm_from_file.h"
#include "do_instructions.h"
#include "processor.h"
#include <stdio.h>

int main(){
    processor baikal = {};
    baikal = init("assembler_task/ass.bin");
    stack_err_bytes a = do_processor_comands(&baikal);
    processor_dump(&baikal);

    // доп таска
    // fill_video_ram(&baikal);
    // show_square(&baikal);

    processor_free(&baikal);
    return 0;
}