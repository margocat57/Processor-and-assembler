#include "processor.h"
#include "parse_asm_from_file.h"
#include "stack_for_calcul/stack_func.h"
#include "stack_for_calcul/mistakes_code.h"
#include "stack_for_calcul/log.h"
#include <stdio.h>
#include <string.h>

processor init(const char* name_of_file){
    processor intel = {};

    if(!name_of_file){
        fprintf(stderr, "NULL pointer to string with file name");
        return intel;
    }

    intel.code = load_code(name_of_file);
    if(!intel.code.comands){
        fprintf(stderr, "NULL pointer to bytecode array");
        return intel;
    }

    intel.stack = stack_ctor(intel.code.size, __FILE__, __func__, __LINE__);
    if(!intel.code.comands){
        fprintf(stderr, "Can't allocate stack memory to stackfor bytecode");
        return intel;
    }

    // пока создаем размером в код так как память дешевая, вопрос как лучше открытый
    intel.call_stack = stack_ctor(intel.code.size, __FILE__, __func__, __LINE__);
    if(!intel.code.comands){
        fprintf(stderr, "Can't allocate stack memory to stack");
        return intel;
    }

    intel.ram_counter = 0;
    intel.ic = 0;
    return intel;
}

void processor_dump(processor* intel){
    printf_to_log_file("Num of elements in bytecode array: %lu\n", intel->code.size);
    printf_to_log_file("Pointer to bytecode element: %lu\n", intel->ic);
    printf_to_log_file("Register max size: %d\n", REGISTR_MAX_SIZE);
    printf_to_log_file("Printing bytecode array:\n");
    for(size_t idx = 0; idx < intel->code.size; idx++){
        printf_to_log_file("[%d]: %d\n", idx, intel->code.comands[idx]);
    }
    printf_to_log_file("Printing registers:\n");
    for(size_t idx_reg = 0; idx_reg < REGISTR_MAX_SIZE; idx_reg++){
        printf_to_log_file("R%cX: %d\n", idx_reg + 'A', intel->registr[idx_reg]);
    }
    //TODO вывод оперативной памяти
    stack_dump(intel->stack);
    // добавить стек адресов возврата
}

stack_err_bytes processor_verify(processor* intel){
    stack_err_bytes error = NO_MISTAKE;
    if(!intel){
        printf_to_log_file("NULL processor pointer\n");
        return INCORR_PROCESSOR_PTR;
    }
    if(!intel->code.comands){
        printf_to_log_file("NULL bytecode array pointer\n");
        return INCORR_BYTECODE_PTR;
    }
    if(intel->ic < 0 || intel->ic > intel->code.size){
        printf_to_log_file("Pointer to bytecode element out of index\n");
        printf_to_log_file("pointer %d\n", intel->ic);
        DEBUG(processor_dump(intel));
        error = error | INCORR_BYTECODE_ELEM_PTR;
    }
    error = error | stack_verify(intel->stack);
    return error;
}

stack_err_bytes processor_free(processor* intel){
    stack_err_bytes err = processor_verify(intel);
    if(err){
        return err;
    }

    // вообще хороший вопрос что лучше занулять указатель(как будто это как раз и делает free)
    stack_free(intel->stack);
    intel->stack = NULL;

    memset(intel->code.comands, 0, intel->code.size * sizeof(int));
    free(intel->code.comands);
    intel->code.comands = NULL;

    stack_free(intel->call_stack);
    intel->call_stack = NULL;

    return NO_MISTAKE;
}
