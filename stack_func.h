#ifndef STACK_FUNC_H
#define STACK_FUNC_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

#define CHECKUS(condition, ReturnNum)                                                     \
        do                                                                                \
        {                                                                                 \
            if (!(condition))                                                             \
            {                                                                             \
                printf("Error: %s, file %s, line %i\n", #condition, __FILE__, __LINE__);  \
                return ReturnNum;                                                         \
            }                                                                             \
        } while (0)

typedef double Elem_t;
#define ELEM_FORMAT "%i"
#define CAPACITY_COEF 2

struct Stack
{
    Elem_t* data;
    int size;
    int capacity;
};


int StackCtor(Stack* stack_p, int size);

int Push(Stack* stack_p, Elem_t var);

int Pop(Stack* stack_p, Elem_t* output_p);

int IncreaseCapacity(Stack* stack_p);

int DecreaseCapacity(Stack* stack_p);

int StackDtor(Stack* stack_p);

#endif
