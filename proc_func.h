#ifndef PROC_FUNC_H
#define PROC_FUNC_H

#include "stack_func.h"

typedef struct Proc_struct
{
    int cn = 0;
    int length = 0;
    Stack* stack_p = NULL;
    Elem_t* RAM = NULL;
    Elem_t* reg = NULL;
    unsigned char* input_data = NULL;
} Proc_struct;




int CheckSignature(FILE* input_file, const int local_version, const char* in_file_name);

int ExecuteCode(Proc_struct* proc_p);

int ExecuteCommand(Proc_struct* proc_p);

int ExecutePush(Proc_struct* proc_p);

int ExecutePop(Proc_struct* proc_p);

#endif
