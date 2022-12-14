#ifndef PROC_FUNC_H
#define PROC_FUNC_H

#include "stack_func.h"
#include "colors.h"
#include <math.h>


typedef struct Proc_struct
{
    int cn = 0;
    int length = 0;
    Stack* stack_p = NULL;
    Elem_t* RAM = NULL;
    Elem_t* reg = NULL;
    unsigned char* input_data = NULL;
} Proc_struct;


const double ZERO_ACCURACY = 0.000001;


int CheckSignature(FILE* input_file, const int local_version, const char* in_file_name);

void ClearInput();

int IsEqual(Elem_t x, Elem_t y);

int ExecuteCode(Proc_struct* proc_p);

int ExecuteCommand(Proc_struct* proc_p);

int ExecutePush(Proc_struct* proc_p);

int ExecutePop(Proc_struct* proc_p);

#endif
