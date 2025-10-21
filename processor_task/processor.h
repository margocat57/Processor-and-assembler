#ifndef PROCESSOR_H
#define PROCESSOR_H
#include "../stack_for_calcul/stack.h"
#include "../stack_for_calcul/mistakes_code.h"

const int REGISTR_MAX_SIZE = 16;
const int RAM_MAX_SIZE = 100;

const int WIDTH = 100;
const int HEIGHT = 100;
const int PIXELSIZE = 5;
const int VIDEO_RAM_MAX_SIZE = WIDTH * HEIGHT * 3;
const int R = 50;

const int WIDTH1 = 50;
const int HEIGHT1 = 50;
const int VIDEO_RAM2_MAX_SIZE = WIDTH1 * HEIGHT1;


struct code_and_size{
    int* comands;
    size_t size;
};

struct processor{
    stack_t_t* stack;
    struct code_and_size code;
    size_t ic;
    int registr[REGISTR_MAX_SIZE];
    stack_t_t* call_stack;
    int RAM[RAM_MAX_SIZE];
    int ram_counter;
    unsigned int VIDEO_RAM[VIDEO_RAM_MAX_SIZE];
    unsigned char VIDEO_RAM2[VIDEO_RAM2_MAX_SIZE];
};

struct res_and_err{
    int res;
    stack_err_bytes proc_err;
};

//! @brief Initializes processor structure and allocates necessary resources
//!
//!  Initialization Sequence:
//! 1. Creates zero-initialized processor structure
//! 2. Validates filename pointer is not NULL
//! 3. Loads bytecode from file into processor's code field
//! 4. Creates main execution stack for operations
//! 5. Creates call stack for function calls and returns
//! 6. Initializes runtime counters:
//!    - ram_counter: Memory address pointer
//!    - ic: Instruction counter (program counter)
//!
//! @par Error Handling:
//! - Returns empty structure if any initialization step fails
//! - Performs cleanup via processor_free() on partial initialization failures
//! - Provides error messages for each failure case
processor init(const char* name_of_file);

void processor_dump(processor* intel);

void bytecode_dump(processor* intel);

void ram_dump(processor* intel);

void reg_dump(processor* intel);

stack_err_bytes processor_verify(processor* intel);

//! @brief Free all processor resources
//!
//! @par Cleanup Sequence:
//! 1. Validates processor pointer is not NULL
//! 2. Frees main execution stack and nullifies pointer
//! 3. Zeroes and frees bytecode array, nullifies pointer  
//! 4. Frees call stack and nullifies pointer
void processor_free(processor* intel);

void fill_video_ram2(processor* intel);

void fill_video_ram(processor* intel);

void show_square(processor* intel);

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