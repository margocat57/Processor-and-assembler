#include "processor.h"
#include "parse_asm_from_file.h"
#include "stack_for_calcul/stack_func.h"
#include <stdio.h>

processor init(const char* name_of_file){
    processor intel = {};
    intel.code = load_code(name_of_file);
    intel.stack = stack_ctor(intel.code.size, __FILE__, __func__, __LINE__);
    intel.ic = 0;
    return intel;
}

void processor_dump(processor* intel){
    printf("Printing code array");
    for(int idx = 0; idx < intel->code.size; idx++){
        printf("[%d]: %d\n", intel->code.comands[idx]);
    }
    stack_dump(intel->stack);
}

void processor_free(processor* intel){
    // TODO check null ptr
    stack_free(intel->stack);
    free(intel->code.comands);
}

//TODO processor_verify

// processor_dump && stack_dump