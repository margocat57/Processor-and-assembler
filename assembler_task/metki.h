#ifndef METKI_H
#define METKI_H
#include "assembler_struct.h"

const int MAX_NUMBER_OF_METKI = 10;

struct metki{
    int* metki_arr;
};

metki metki_init(assembler* assembl);

void metki_destroy(assembler* assembl);

#endif //METKI_H