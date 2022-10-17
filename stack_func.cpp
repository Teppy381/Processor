#include "stack_func.h"

int StackCtor(Stack* stack_p, int size)
{
    stack_p->data = (Elem_t*) calloc(size, sizeof(Elem_t));
    stack_p->size = 0;
    stack_p->capacity = size;

    return 0;
}

int Push(Stack* stack_p, Elem_t var)
{
    if (stack_p->size >= stack_p->capacity)
    {
        IncreaseCapacity(stack_p);
    }

    (stack_p->data)[stack_p->size] = var;
    stack_p->size++;

    //printf("Push: size = %i, capacity = %i\n", stack_p->size, stack_p->capacity);

    return 0;
}


int Pop(Stack* stack_p, Elem_t* output_p)
{
    if (stack_p->size < 1)
    {
        printf("Cannot pop one more time, stack is empty\n");
        return 1;
    }

    *output_p = stack_p->data[stack_p->size - 1];
    DecreaseCapacity(stack_p);
    stack_p->size--;

    //printf("Pop:  size = %i, capacity = %i\n", stack_p->size, stack_p->capacity);

    return 0;
}


int IncreaseCapacity(Stack* stack_p)
{
    stack_p->data = (Elem_t*) realloc(stack_p->data, CAPACITY_COEF * stack_p->capacity * sizeof(Elem_t));
    stack_p->capacity = stack_p->capacity * CAPACITY_COEF;

    //printf("Capacity increased: %i\n", stack_p->capacity);

    return 0;
}


int DecreaseCapacity(Stack* stack_p)
{
    if (stack_p->capacity <= stack_p->size * CAPACITY_COEF)
    {
        return 1;
    }

    stack_p->data = (Elem_t*) realloc(stack_p->data, stack_p->capacity * sizeof(*stack_p->data) / CAPACITY_COEF);
    stack_p->capacity = stack_p->capacity / CAPACITY_COEF;

    //printf("Capacity decreased: %i\n", stack_p->capacity);

    return 0;
}


int StackDtor(Stack* stack_p)
{
    free(stack_p->data);
    return 0;
}

