#ifndef PROCESSOR_H
#define PROCESSOR_H
#include "stack_for_calcul/stack.h"
#include "stack_for_calcul/mistakes_code.h"

const int REGISTR_MAX_SIZE = 16;

struct code_and_size{
    int* comands;
    size_t size;
};

struct processor{
    stack_t_t* stack;
    struct code_and_size code;
    size_t ic;
    int registr[REGISTR_MAX_SIZE];
};

struct res_and_err{
    int res;
    stack_err_bytes proc_err;
};

processor init(const char* name_of_file);

void processor_dump(processor* intel);

stack_err_bytes processor_verify(processor* intel);

void processor_free(processor* intel);

enum registr{
    RAX = 0,
    RBX = 1,
    RCX = 2,
    RDX = 3,
    REX = 4,
    RFX = 5,
    RGX = 6,
    RHX = 7,
    RIX = 8,
    RJX = 9,
    RKX = 10, 
    RLX = 11,
    RMX = 12,
    RNX = 13,
    ROX = 14,
    RPX = 15
};

#endif // PROCESSOR_H